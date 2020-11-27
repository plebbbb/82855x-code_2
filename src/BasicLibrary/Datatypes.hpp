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
    - The radians, degrees, and SMART_radians pattern is kinda bad.
      Doesn't allow for conversion from radian to degrees or smart radians to radians
*/


/**************************************************************************************************/
// ROTATIONAL UNITS

/*A smart radian automatically prunes all values to be within the range of 0 - 2PI
Its a radian, but smart*/
struct SMART_radians{
  double value; //This holds your angle value, in radians


  /******************************************************************************/
  //Constructors:
  SMART_radians(){value = 0;} //default constructor, if you don't make it anything it's 0 by default

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

      //Division by zero correction is to set angle to 0
        if (isnanf(value) || isinff(value)) value = 0;

      //Float modulus to correct angle values to the right inte
        value = fmod(value, M_PI*2);
    }

  /******************************************************************************/
  //Conversion functions
  operator double() {
      prune();
      return value;
  }


  /******************************************************************************/
  //Manipulation functions
  void operator+=(SMART_radians increment) {
      value += increment;
      prune();
  }
};


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

    explicit operator SMART_radians(){
      return SMART_radians(value);
    }


    /******************************************************************************/
    //Manipulation functions
    void operator+=(radians increment) {
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

    //you must explicitly state that you are converting to a smart radian before you do so
    explicit operator SMART_radians(){
      return SMART_radians(value * M_PI / 180);
    }


    /******************************************************************************/
    //Manipulation functions
    void operator+=(degrees increment) {
        value+=increment;
        //prune();
    }

};



/**************************************************************************************************/
// LINEAR UNITS

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
  //it has to be noted that I can't convert an inches to a radians, or a degrees, as that is two layers of implicit conversion
  //you are allowed to do only a single layer of implicit conversion, everything after that must be explicitly stated.
  /*for example,
    *degrees variable* = (degrees)((double)*an inch variable*) is legal
    *degrees variable* = (degrees)(*an inch variable*) is also legal, as you implicitly convert the inch into a double
    *degrees variable* = *an inch variable* is not legal, as there are two conversions
  */

  /******************************************************************************/
  //Manipulation functions
  void operator+=(inches increment){
    value += increment;
  }

  void operator-=(inches increment){
    value-= increment;
  }
};


/**************************************************************************************************/
// PROPORTIONAL UNITS

//percent, features to be added when needed
struct percent{
  double value;

  /******************************************************************************/
  //Constructors:
  percent(){value = 0;} //default constructor, if you don't make it anything it's 0 by default

  percent(double in){
    value = in;
  }
  inches operator=(double in){
    value = in;
  }


  /******************************************************************************/
  //Conversion functions:
  operator double() {
    return value;
  }
  //it has to be noted that I can't convert an inches to a radians, or a degrees, as that is two layers of implicit conversion
  //you are allowed to do only a single layer of implicit conversion, everything after that must be explicitly stated.
  /*for example,
    *degrees variable* = (degrees)((double)*an inch variable*) is legal
    *degrees variable* = (degrees)(*an inch variable*) is also legal, as you implicitly convert the inch into a double
    *degrees variable* = *an inch variable* is not legal, as there are two conversions
  */

  /******************************************************************************/
  //Manipulation functions
  void operator+=(percent increment){
    value += increment;
  }

  void operator-=(percent increment){
    value-= increment;
  }
};


/**************************************************************************************************/
// COMPOSITE UNITS

enum AXIS_VALUES{
  X_AXIS = 0,
  Y_AXIS = 1,
  ROTATION = 2
};

//standard position datatype, holds an x, y, and an orientation
struct position{
  inches x; //x coordinate
  inches y; //y coordinate
  SMART_radians angle; //Smart radian to constrain to 0 - 2*PI


  /******************************************************************************/
  //Constructors:
  /*The constructor for positons uses a std::tuple
    How you format those is like so {x, y, angle}*/
  position(std::tuple<inches, inches, SMART_radians> set):
  x(std::get<0>(set)),
  y(std::get<1>(set)),
  angle(std::get<2>(set)){}


  position operator= (std::tuple<inches, inches, SMART_radians> set){
    return position(set);
  }

  position(std::pair<inches, inches> set):
    x(std::get<0>(set)),
    y(std::get<1>(set)){angle = 0;}

  position() {x = 0; y = 0; angle = 0;} //default constructor assumes you are at origin facing pos x axis

  /******************************************************************************/
  //Conversion functions
  operator std::tuple<inches,inches,SMART_radians>(){
    return {x,y,angle};
  }

  operator std::pair<inches,inches>(){
    return {x,y};
  }


  /******************************************************************************/
  //Manipulation functions
  /*Remember that an conversion from tuples({x,y,angle}) to position can automatically happen
  Using that instead of an actual position type is also perfectly legal*/
  void operator+=(position change){
    x += change.x;
    y += change.y;
    angle += change.angle; //Smart radians automatically deal with out of bound possiblities
  }

  /*REMEMBER TO CONVERT BACK AFTER!!,
  Also, this is a dumb hack that violates the whole purpose of these datatypes*/
  double operator[](AXIS_VALUES index){
    switch (index){
      case X_AXIS: return x;
      case Y_AXIS: return y;
      case ROTATION: return angle;
    }
  }
};

//Standard coordinate datatype, holds coordinates of arbritrary vector in R2
struct coordinate{
  inches x;
  inches y;
  inches length;
  coordinate(std::pair<inches,inches> set):x(set.first),y(set.second) {get_length();}
  coordinate operator=(std::pair<inches,inches> set){
    return coordinate(set);
  }
  coordinate(){x = 0; y = 0;}

  /******************************************************************************/
  //Utility functions

  //We convert our cartesian coordinates into relative
  void self_transform_polar(SMART_radians offset){
    get_length();
    SMART_radians polar_angle(atan2(y,x) + offset);
    x = length*cos(polar_angle);
    y = length*sin(polar_angle);
  }

  //updates length of coordinate
  inches get_length(){
    length = sqrt(x*x+y*y);
    return length;
  }

  /******************************************************************************/
  //Conversion functions
  operator position(){
    return std::pair<inches,inches>{x,y};
  }

  /******************************************************************************/
  //Manipulation functions
  /*Remember that an conversion from pairs({x,y}) to coordinate can automatically happen
  Using that instead of an actual coordinate type is also perfectly legal*/
  void operator+=(position change){
    x += change.x;
    y += change.y;
    get_length();
  }

  inches operator[](AXIS_VALUES index){
    switch (index){
      case X_AXIS: return x;
      case Y_AXIS: return y;
    }
  }


  };
}

#endif
