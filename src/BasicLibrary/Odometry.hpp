//A header guard
#pragma once
#include "Library.hpp"
//Library.hpp imports all of Odometry.hpp's code, and here, Odometry.hpp imports library.hpp.
//You may think that this would lead to the program importing itself in an infinite loop,
//but we have the header guard set up. This way, Odometry is only looped over once


//BasicLibrary is what we will call our library.
//To refrence anything from the library, we need to have the name followed by a ::, then the requested item.
//For example, BasicLibrary::Somefunction() would work, Somefunction() would not work.
namespace BasicLibrary{
  class DeadWheel;
  struct OdometryWheels;
  static double DegreesToRadians(double Degrees);

  //Ok Cho, I just made the encoder classes. How you want to interface with these is up to you.
  //unique_ptr.get() gives you the pointer for the array of values. Put everything below odometrywheels to be safe/

  //This is a wrapper class that serves as another class with additional features
  //Here, we provide the feature to automatically convert the angle value of a shaft
  //encoder into the traversed distance by it's attached wheel.

  class DeadWheel{
    pros::ADIEncoder Encoder; //The shaft encoder
    double WheelRadius; //The radius of the deadwheel
    double Distance_CenterOfRotation;

  //The stuff above public cannot be accessed externally

  public:

    //Constructor if the encoder is plugged directly into the V5 Brain ADI ports
    explicit DeadWheel(int portA, int portB, bool direction, float Diameter):
    Encoder(portA,portB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Constructor if the encoder is plugged into an ADI Expander
    //The explicit keyword allows for the compiler to automatically convert data specified in incorrect data types to their correct ones
    //An example would be people using integers to specify ports for vex sensors. They actually use their own fancy data type
    explicit DeadWheel(pros::ext_adi_port_tuple_t portAB, bool direction, float Diameter):
    Encoder(portAB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Returns us the raw angle traversed by the encoder in radians
    double get_radian(){
      return DegreesToRadians(Encoder.get_value());
    }

    //Returns us the effective distance traveled by the attached wheel
    double get_distance(){
      return WheelRadius*get_radian();
    }

    //get_distance. Note that this resets the encoder as well. Do not call this multiple times per cycle
    double get_distance_AUTORESET(){
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
    std::unique_ptr<std::array<double, 3>> get_distances(){
      return std::unique_ptr<std::array<double, 3>>(
        new std::array<double, 3> {
          LEFT.get_distance_AUTORESET(),
          RIGHT.get_distance_AUTORESET(),
          BACK.get_distance_AUTORESET()
        }
      );
    }

    //This function basically takes control of the [] thing you see in arrays
    //This turns the object into a psudo-array thats basically an array
    //Usable values are index 0, 1 2. or 'L', 'R', 'B', or those but lowercase
    //FYI, this doesn't reset, so you will have to manual reset with the reset function
    double operator[] (int index){
        switch(index){
            case 'L':
            case 'l':
            case 0: return LEFT.get_distance();
            case 'R':
            case 'r':
            case 1: return RIGHT.get_distance();
            case 'B':
            case 'b':
            case 2: return BACK.get_distance();
        };
    }

  };

  //Converts degrees into radians. Radius*radians = distance traveled
  static double DegreesToRadians(double Degrees){
    return Degrees*(180/M_PI);
  };
}
