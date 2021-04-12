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
		motorwheel(pros::Motor(18,false),2.75,3*M_PI/4), //top right corner
		motorwheel(pros::Motor(5,false),2.75,5*M_PI/4),	//top left corner
		motorwheel(pros::Motor(6,false),2.75,7*M_PI/4),	//bottom left corner
		motorwheel(pros::Motor(15,false),2.75,M_PI/4)		//bottom right corner
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

Imu LIM(16);
///*
OdometryWheels Owheels{
  DeadWheel({7,'E','F'},true,inches(2.7703),6.24176), //LEFT
  DeadWheel({7,'A','B'},true,inches(2.7534),6.4186088), //RIGHT
  DeadWheel({7,'C','D'},true,inches(2.7225),4.24502)  //BACK
};

DSensorComputer DSodom(
	DSensor(4, M_PI*5/4, std::pair<inches,inches>{-8,6}), //left corner distance sensor
	DSensor(17, M_PI*7/4, std::pair<inches,inches>{8,6})   //right corner distance sensor
);


OdometryComputer Odom = OdometryComputer(Owheels);
//*/

bool balltransferstate = false;

linetracker bottom(ADIAnalogIn({7,'G'}),2800);
linetracker top(ADIAnalogIn('D'),2800);
linetracker middle(ADIAnalogIn({7,'H'}),2800);
linetracker eject(ADIAnalogIn('E'),2800);
Optical input(8);

bool toggleglobaldrive = true;

Motor Lintake(9);
Motor Rintake(19,true);
Motor Shooter(20);
Motor Ejector(14);


intakecontroller inta(Lintake,Rintake,Ejector,bottom);
scorecontroller scra(Ejector,Shooter,top);

unifiedliftcontroller ttt(input, {top,middle,bottom}, eject, inta, scra);
