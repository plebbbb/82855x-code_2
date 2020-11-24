//A header guard
#pragma once
//#include "Library.hpp"
//Library.hpp imports all of Odometry.hpp's code, and here, Odometry.hpp imports library.hpp.
//You may think that this would lead to the program importing itself in an infinite loop,
//but we have the header guard set up. This way, Odometry is only looped over once
//#include "Datatypes.hpp"

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

    //Constructor if the encoder is plugged directly into the V5 Brain ADI ports
    DeadWheel(int portA, int portB, bool direction, double Diameter):
    Encoder(portA,portB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Constructor if the encoder is plugged into an ADI Expander
    DeadWheel(pros::ext_adi_port_tuple_t portAB, bool direction, double Diameter):
    Encoder(portAB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Returns us the raw angle traversed by the encoder in radians
    radians get_radian(){
      return degrees(Encoder.get_value());
    }

    //Returns us the effective distance traveled by the attached wheel
    inches get_distance(){
      return WheelRadius*get_radian();
    }

    //get_distance. Note that this resets the encoder as well. Do not call this multiple times per cycle
    inches get_distance_AUTORESET(){
      double val = get_distance();
      reset();
      return val;
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
    DeadWheel LEFT, RIGHT, BACK;

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

    /*This function basically takes control of the [] thing you see in arrays.
    This turns the object into a psudo-array thats basically an array.
    Usable values are index 0, 1 2. or 'ENCODER_POSITION_LEFT', right and center.
    FYI, this doesn't reset, so you will have to manual reset with the reset function.*/
    double operator[] (ENCODER_POSITION index){
        switch(index){
            case 0: return LEFT.get_distance();
            case 1: return RIGHT.get_distance();
            case 2: return BACK.get_distance();
        };
    }

    void reset(){
      LEFT.reset();
      RIGHT.reset();
      BACK.reset();
    }
  };

}

#endif
