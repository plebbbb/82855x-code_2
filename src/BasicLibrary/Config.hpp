//A header guard
#pragma once

#ifndef CONFIG_HPP
#define CONFIG_HPP

//Janky HPP for forward declerations. This pattern is questionable
//So I have basically no idea if this is big brain or stupid

//GENERAL IMPORTANT THINGS **************************************************/

extern pros::Controller master;

extern STL_lib::position locationC;

extern STL_lib::command currentcommand;

//**************************************************************************//

//BASE THINGS ***************************************************************/

extern STL_lib::holonomicbase base;

extern STL_lib::basecontroller_auton autonbase;


//**************************************************************************//

//ODOM THINGS ***************************************************************/

extern pros::Imu LIM;
extern pros::Imu RIM;

extern STL_lib::OdometryWheels Owheels;

extern STL_lib::OdometryComputer Odom;

extern STL_lib::DSensorComputer DSodom;

//**************************************************************************//

//TEMP THINGS ***************************************************************/]

extern bool toggleglobaldrive;

extern pros::Motor Lintake;
extern pros::Motor Rintake;
extern pros::Motor Shooter;
extern pros::Motor Ejector;

extern STL_lib::linetracker bottom;
extern STL_lib::linetracker top;
extern STL_lib::linetracker middle;


extern STL_lib::intakecontroller inta;
extern STL_lib::scorecontroller scra;


#endif
