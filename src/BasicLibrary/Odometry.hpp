//A header guard
#pragma once
//#include "Library.hpp"
//Library.hpp imports all of Odometry.hpp's code, and here, Odometry.hpp imports library.hpp.
//You may think that this would lead to the program importing itself in an infinite loop,
//but we have the header guard set up. This way, Odometry is only looped over once
//#include "Datatypes.hpp"

/*TBD: Changes are needed to the current class linking system
  - OdometryWheels should be a child class of odometrycontroller
    - What this allows is for us to make different odometry configurations
      For example, an odometry variation that takes the IMU, X, and Y values
*/

#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

namespace STL_lib{

//BasicLibrary is what we will call our library.
//To refrence anything from the library, we need to have the name followed by a ::, then the requested item.
//For example, BasicLibrary::Somefunction() would work, Somefunction() would not work.
  struct DeadWheel;
  struct OdometryWheels;
  struct OdometryComputer;

  enum ENCODER_POSITION{
    ENCODER_POSITION_LEFT = 0,
    ENCODER_POSITION_RIGHT = 1,
    ENCODER_POSITION_BACK = 2
  };


  //An ADI encoder wrapper that directly outputs distance values
  struct DeadWheel{
    pros::ADIEncoder Encoder; //The shaft encoder
    inches WheelRadius; //The radius of the deadwheel
    inches Distance_CenterOfRotation; //Distance to center of rotation
    int encoderTotal = 0; // Stores total encoder value

    //Constructor if the encoder is plugged directly into the V5 Brain ADI ports
    DeadWheel(int portA, int portB, bool direction, inches Diameter, inches Dist_to_ctr):
    Encoder(portA,portB,direction)
    {
      WheelRadius = inches((double)Diameter/2.000000);
      Distance_CenterOfRotation = Dist_to_ctr;
    }

    //Constructor if the encoder is plugged into an ADI Expander
    DeadWheel(pros::ext_adi_port_tuple_t portAB, bool direction, inches Diameter, inches Dist_to_ctr):
    Encoder(portAB,direction)
    {
      WheelRadius = inches((double)Diameter/2.000000);
      Distance_CenterOfRotation = Dist_to_ctr;
    }

    //Returns us the raw angle traversed by the encoder in radians
    radians get_radian(){
      return radians(degrees(Encoder.get_value()));
    }

    //Returns us the effective distance traveled by the attached wheel
    inches get_distance(){
      return inches((double)WheelRadius*(double)get_radian());
    }

    //get_distance. Note that this resets the encoder as well. Do not call this multiple times per cycle
    inches get_distance_AUTORESET(){
      inches val = get_distance();
      encoderTotal += get_distance(); // Add encoder's value to a total
      reset();
      return val;
    }

    // Returns radian value of the encoder total
    radians get_radianTotal(){
      return radians(degrees(encoderTotal));
    }

    // Returns total distance via the encoder total
    inches get_distanceTotal(){
      return inches((double)WheelRadius*(double)get_radianTotal());
    }

    void reset(){
      Encoder.reset();
    }
  };

  //This is a struct meant to hold the DeadWheels in an convienent to move container
  //Treat it like an array that has built in functions. It can literally do the [] and the {}

  struct OdometryWheels{
    //This struct is constructed with bracket notation.
    //For example, DeadWheel a = {LEFT, RIGHT, REAR};
    DeadWheel LEFT;
    DeadWheel RIGHT;
    DeadWheel BACK;
//    OdometryWheels(DeadWheel L, DeadWheel R, DeadWheel B):LEFT(L),RIGHT(R),BACK(B){}

    //This approach only measures the encoder values once per cycle, ensuring synchronization of measurements
    std::unique_ptr<std::array<inches, 3>> get_distances(){
      return std::unique_ptr<std::array<inches, 3>>(
        new std::array<inches, 3> {
          LEFT.get_distance_AUTORESET(),
          RIGHT.get_distance_AUTORESET(),
          BACK.get_distance_AUTORESET()
        }
      );
    }

    std::array<inches,3> get_distances_nonpointer(){
      return std::array<inches, 3>{
        LEFT.get_distance_AUTORESET(),
        RIGHT.get_distance_AUTORESET(),
        BACK.get_distance_AUTORESET()
      };
    }

    // Returns array of total encoder distances
    std::array<inches,3> get_distancesTotal_nonpointer(){
      return std::array<inches, 3>{
        LEFT.get_distanceTotal(),
        RIGHT.get_distanceTotal(),
        BACK.get_distanceTotal()
      };
    }
    /*This function basically takes control of the [] thing you see in arrays.
    This turns the object into a psudo-array thats basically an array.
    Usable values are index 0, 1 2. or 'ENCODER_POSITION_LEFT', right and center.
    FYI, this doesn't reset, so you will have to manual reset with the reset function.*/
    inches DistOf(ENCODER_POSITION index){
        switch(index){
            case ENCODER_POSITION_LEFT: return LEFT.get_distance();
            case ENCODER_POSITION_RIGHT: return RIGHT.get_distance();
            case ENCODER_POSITION_BACK: return BACK.get_distance();
        };
    }

    DeadWheel operator[] (ENCODER_POSITION index){
      switch(index){
          case ENCODER_POSITION_LEFT: return LEFT;
          case ENCODER_POSITION_RIGHT: return RIGHT;
          case ENCODER_POSITION_BACK: return BACK;
      };
    }

    void reset(){
      LEFT.reset();
      RIGHT.reset();
      BACK.reset();
    }
  };


  //Odometry class that actually does the calculations
  struct OdometryComputer{
    OdometryWheels wheels;

    /******************************************************************************/
    //Constructors:
    OdometryComputer(OdometryWheels wheelset):wheels(wheelset){}


    /******************************************************************************/
    //Utility functions
    /*The edge case of a division by zero distance is only observed in odometry
    Hence, we have this janky internal function to deal with these situations*/
    double divzerocomp(double numerator, double denomator){
      if (numerator == 0 || denomator == 0) return 0.0;
      return numerator/denomator;
    }



    /******************************************************************************/
    //Primary functions
    position cycle(position precycle){
      std::array EncoderDistanceValues = wheels.get_distances_nonpointer();
      //its a 50/50 that get_distances_nonpointer works
      std::array EncoderDistanceTotalValues = wheels.get_distancesTotal_nonpointer();

      //Assuming forwards is 0rad, CCW is positive we calculate the relative offset
      //All coords are prior to move fyi.
      radians raw_global_angle =
        (EncoderDistanceValues[0]-EncoderDistanceValues[1]) /
        (wheels[ENCODER_POSITION_LEFT].Distance_CenterOfRotation +
        wheels[ENCODER_POSITION_RIGHT].Distance_CenterOfRotation);

      radians rel_orientation_change = precycle.angle() -
        (EncoderDistanceTotalValues[0]-EncoderDistanceTotalValues[1]) /
        (wheels[ENCODER_POSITION_LEFT].Distance_CenterOfRotation +
        wheels[ENCODER_POSITION_RIGHT].Distance_CenterOfRotation);

      //SMART_radians' built in interval restriction isn't needed here. We need negative intervals.

      coordinate returncycle(std::pair<inches,inches>{0,0});

      radians avg_angle = rel_orientation_change/2.0;

      if (rel_orientation_change == 0){
        returncycle.x = EncoderDistanceValues[2];
        returncycle.y = EncoderDistanceValues[1];
      } else {
        returncycle.y = inches(2.0*sin(avg_angle) *
        ((EncoderDistanceValues[1]/rel_orientation_change) +
        wheels[ENCODER_POSITION_RIGHT].Distance_CenterOfRotation));

        returncycle.x = inches(2.0*sin(avg_angle) *
        ((EncoderDistanceValues[2]/rel_orientation_change) +
        wheels[ENCODER_POSITION_BACK].Distance_CenterOfRotation));
      }

      returncycle = returncycle.transform_matrix(-(precycle.angle+avg_angle-(M_PI/2)));

      precycle += returncycle;
      precycle.angle = raw_global_angle;

      return precycle;
    }
  };

}

#endif
