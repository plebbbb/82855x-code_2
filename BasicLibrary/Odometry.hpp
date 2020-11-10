//A header guard to prevent duplicate declarations. Prevents everything between this and a #endif line from being duped
#ifndef Odometry_HPP
#define Odometry_HPP

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
  //unique_ptr.get() gives you the pointer for the array of values

  struct OdometryWheels{
    //This struct is constructed with bracket notation.
    //For example, DeadWheel a = {LEFT, RIGHT, REAR};
    DeadWheel LEFT, RIGHT, REAR;

    //This approach only measures the encoder values once per cycle, ensuring synchronization of measurements
    std::unique_ptr<std::array<double, 3>> get_distances(){
      return std::unique_ptr<std::array<double, 3>>(
        new std::array<double, 3> {
          LEFT.get_distance_AUTORESET(),
          RIGHT.get_distance_AUTORESET(),
          REAR.get_distance_AUTORESET()
        }
      );
    }
  };

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
    DeadWheel(int portA, int portB, bool direction, float Diameter):
    Encoder(portA,portB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Constructor if the encoder is plugged into an ADI Expander
    //The explicit keyword allows for the compiler to automatically convert data specified in incorrect data types
    //Explicit uses functions that take one data type, and return another data type to reach the final value
    explicit DeadWheel(pros::ext_adi_port_tuple_t portA, pros::ext_adi_port_tuple_t portB, bool direction, float Diameter):
    Encoder(portA,portB,direction)
    {
      WheelRadius = Diameter/2;
    }

    //Returns us the raw angle traversed by the encoder in radians
    double get_radian(){
      return DegreesToRadians(get_angle();
    }

    //Returns us the effective distance traveled by the attached wheel
    double get_distance(){
      return WheelRadius*get_angle();
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

  //Converts degrees into radians. Radius*radians = distance traveled
  static double DegreesToRadians(double Degrees){
    return Degrees*(180/M_PI);
  };
}


#endif //Everything from here to Odometry_HPP is guarded
