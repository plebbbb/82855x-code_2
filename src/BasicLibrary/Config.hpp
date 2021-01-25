//A header guard
#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

//Janky HPP for forward declerations. This pattern is questionable
//So I have basically no idea if this is big brain or stupid

//GENERAL IMPORTANT THINGS **************************************************/

extern pros::Controller master;

extern STL_lib::SMART_radians globalangle;


//**************************************************************************//

//BASE THINGS ***************************************************************/

extern STL_lib::holonomicbase base;

extern STL_lib::basecontroller_auton autonbase;


//**************************************************************************//

//ODOM THINGS ***************************************************************/

extern pros::Imu im;

extern STL_lib::OdometryWheels Owheels;

extern STL_lib::OdometryComputer Odom;


//**************************************************************************//

//TEMP THINGS ***************************************************************/]

extern bool toggleglobaldrive;

extern pros::Motor Lintake;
extern pros::Motor Rintake;
extern pros::Motor Shooter;
extern pros::Motor Ejector;




#endif
