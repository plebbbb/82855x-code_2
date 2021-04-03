#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;

//GENERAL IMPORTANT THINGS **************************************************/

Controller master(pros::E_CONTROLLER_MASTER);

position locationC = *new position({0,0,M_PI/2});

command currentcommand(position({15,15,M_PI}),{5,20,15},{0,0},{0,0});

//**************************************************************************//
//BASE THINGS ***************************************************************/

holonomicbase base(
	{
		motorwheel(pros::Motor(20,false),2.75,3*M_PI/4), //top right corner
		motorwheel(pros::Motor(16,false),2.75,5*M_PI/4),	//top left corner
		motorwheel(pros::Motor(17,false),2.75,7*M_PI/4),	//bottom left corner
		motorwheel(pros::Motor(19,false),2.75,M_PI/4)		//bottom right corner
	}
);

basecontroller_auton autonbase(
  base,
  control_loop({new Proportional(40,{100,-100}),new Derivitive(35,{70,-70})},{100,-100}),  //X control loop
  control_loop({new Proportional(40,{100,-100}),new Derivitive(35,{70,-70})},{100,-100}),  //Y control loop
  control_loop({new Proportional(200,{100,-100}),new Derivitive(80,{20,-20})},{100,-100}),   //Rotation control loop
	control_loop({new Proportional(1,{0.5,-0.5}),new Derivitive(0.4,{0.25,-0.25})},{0.5,-0.5})   //Rotation control loop, vector mode
);
//, new Derivitive(1,{0.5,-0.5})},{1,-1}
//**************************************************************************//

//ODOM THINGS ***************************************************************/

Imu LIM(15);
///*
OdometryWheels Owheels{
  DeadWheel({6,'E','F'},true,inches(2.7703),6.24176), //LEFT
  DeadWheel({6,'A','B'},true,inches(2.7534),6.4186088), //RIGHT
  DeadWheel({6,'C','D'},true,inches(2.7225),4.24502)  //BACK
};

DSensorComputer DSodom(
	DSensor(1, M_PI*5/4, std::pair<inches,inches>{-8,6}), //left corner distance sensor
	DSensor(15, M_PI*7/4, std::pair<inches,inches>{8,6})   //right corner distance sensor
);


OdometryComputer Odom = OdometryComputer(Owheels);
//*/

bool balltransferstate = false;

linetracker bottom(ADIAnalogIn({6,'G'}),2800);
linetracker top(ADIAnalogIn('G'),2800);
linetracker middle(ADIAnalogIn({6,'H'}),2800);
linetracker eject(ADIAnalogIn('H'),2800);
Optical input(3);

bool toggleglobaldrive = true;

Motor Lintake(7);
Motor Rintake(8,true);
Motor Shooter(9);
Motor Ejector(10);


intakecontroller inta(Lintake,Rintake,Ejector,bottom);
scorecontroller scra(Ejector,Shooter,top);
