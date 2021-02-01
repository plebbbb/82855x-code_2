#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;

//GENERAL IMPORTANT THINGS **************************************************/

Controller master(pros::E_CONTROLLER_MASTER);

position locationC({0,0,M_PI/2});

command currentcommand(position({15,15,M_PI}),{6,25,50},{0,0},{0,0});


//**************************************************************************//

//BASE THINGS ***************************************************************/

holonomicbase base(
	{
		motorwheel(pros::Motor(20,true),3.25,3*M_PI/4), //top right corner
		motorwheel(pros::Motor(15,true),3.25,5*M_PI/4),	//top left corner
		motorwheel(pros::Motor(17,true),3.25,7*M_PI/4),	//bottom left corner
		motorwheel(pros::Motor(19,true),3.25,M_PI/4)		//bottom right corner
	}
);

basecontroller_auton autonbase(
  base,
  control_loop({new Proportional(70,{100,-100})},{100,-100}),  //X control loop
  control_loop({new Proportional(70,{100,-100})},{100,-100}),  //Y control loop
  control_loop({new Proportional(150,{100,-100})},{100,-100}),   //Rotation control loop
	control_loop({new Proportional(1,{0.9,-0.9}),new Derivitive(0.6,{0.5,-0.5})},{0.9,-0.9})   //Rotation control loop
);
//, new Derivitive(1,{0.5,-0.5})},{1,-1}
//**************************************************************************//

//ODOM THINGS ***************************************************************/

Imu im(16);

OdometryWheels Owheels{
  DeadWheel({3,'A','B'},false,inches(2.7881),5.8046),
  DeadWheel({3,'G','H'},true,inches(2.7535),5.7565),
  DeadWheel({3,'C','D'},true,inches(2.8093),5.9819)
};

OdometryComputer Odom = OdometryComputer(Owheels);




bool toggleglobaldrive = true;
Motor Lintake(8);
Motor Rintake(9,true);
Motor Shooter(6);
Motor Ejector(10,true);

intakecontroller inta(Lintake,Rintake,Ejector);
scorecontroller scra(Ejector,Shooter);
