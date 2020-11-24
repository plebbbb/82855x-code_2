//A header guard
#pragma once
//#include "Library.hpp"
struct radians;
struct degrees;
struct inches;

#ifndef DATATYPES_HPP
#define DATATYPES_HPP

namespace STL_lib{

/*
These wrappers may appear to be overkill when directly using ints and doubles are enough,
but they help with organization to an extent, and automatically deal out conversions
*/

/*Things to fix:
    - This pattern is kinda bad. Doesn't allow for conversion from radian to degrees
*/

//A radians datatype, which holds a radians value
struct radians {
    double value; //This holds your angle value, in radians

    /******************************************************************************/
    //Constructors:
    radians(){value = 0;} //default constructor, if you don't make it anything it's 0 by default

    radians(double angle_in_radians) {
        value = angle_in_radians;
        //prune();
    }

    radians operator=(double angle_in_radians) {
        value = angle_in_radians;
    //    prune();
    }


    /******************************************************************************/
    //Utility functions
    void prune() { //constrains the angle values into the range of 0 - 2PI
            value = fmod(value, M_PI*2);
      }


    /******************************************************************************/
    //Conversion functions

    operator double() {
        return value;
    }


    /******************************************************************************/
    //Manipulation functions
    void operator+=(double increment) {
        value += increment;
    //    prune();
    }
};

//A degrees datatype, which holds a degree value
struct degrees {
    double value; //This holds your angle value, in degrees

    /******************************************************************************/

    //Constructors:
    degrees(){value = 0;} //default constructor, if you don't make it anything it's 0 by default

    degrees(double angleindegrees) {
        value = angleindegrees;
      //  prune();
    }

    degrees operator=(double angleindegrees) {
        value = angleindegrees;
        //prune();
    }

    /******************************************************************************/


    //Utility functions
    void prune(){ //constrains the angle values into the range of 0 - 360 degrees
        value = fmod(value, 360);
    }

    /******************************************************************************/

    //Conversion functions
    operator double() {
        return value;
    }

    operator radians() {
        return radians(value * M_PI / 180);
    }

    /******************************************************************************/

    //Manipulation functions
    void operator+=(double increment) {
        value+=increment;
        //prune();
    }

};

//A smart radian automatically prunes all values to be within the range of 0 - 2PI
struct SMART_radians{
  double value; //This holds your angle value, in radians

  /******************************************************************************/

  //Constructors:
  SMART_radians(double angle_in_radians) {
      value = angle_in_radians;
      prune();
  }

  SMART_radians operator=(double angle_in_radians) {
      value = angle_in_radians;
      prune();
  }

  /******************************************************************************/

  //Utility functions
  void prune() { //constrains the angle values into the range of 0 - 2PI
          value = fmod(value, M_PI*2);
    }

  /******************************************************************************/


  //Conversion functions

  operator double() {
      return value;
  }


  /******************************************************************************/


  //Manipulation functions
  void operator+=(double increment) {
      value += increment;
      prune();
  }
};

//inches, I'll implement cm later
struct inches{
  double value;

  /******************************************************************************/
  //Constructors:
  inches(){value = 0;} //default constructor, if you don't make it anything it's 0 by default

  inches(double distance){
    value = distance;
  }
  inches operator=(double distance){
    value = distance;
  }


  /******************************************************************************/
  //Conversion functions:
  operator double() {
    return value;
  }


  /******************************************************************************/
  //Manipulation functions
  void operator+=(double increment){
    value += increment;
  }

  void operator-=(double increment){
    value-= increment;
  }
};

}

#endif
