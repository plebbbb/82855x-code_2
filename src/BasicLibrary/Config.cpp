#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;

//GENERAL IMPORTANT THINGS **************************************************/

Controller master(pros::E_CONTROLLER_MASTER);

position locationC = *new position({36,9,M_PI/2});

command currentcommand(position({15,15,M_PI}),{4,30,15},{0,0},{0,0});

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
	control_loop({new Proportional(41,{100,-100}),new Derivitive(37,{75,-75})},{100,-100}),  //X control loop
  control_loop({new Proportional(41,{100,-100}),new Derivitive(37,{75,-75})},{100,-100}),  //Y control loop
  control_loop({new Proportional(200,{100,-100}),new Derivitive(80,{20,-20})},{100,-100}),   //Rotation control loop
	control_loop({new Proportional(1.1,{0.5,-0.5}),new Derivitive(2,{0.65,-0.65})},{0.5,-0.5})   //Rotation control loop, vector mode
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


//NOTE: THESE OFFSETS ARE WRONG BUT WE ALREADY HAVE THE BOT AUTON SET UP FOR THESE VALUES
//THE CORRECT VALUES ARE: X: 7.75, Y: 6.25
//THIS ONLY AFFECTS CORNER GOAL DISTANCE CALCULATIONS
DSensorComputer DSodom(
	DSensor(4, M_PI*5/4, std::pair<inches,inches>{-8,6}), //left corner distance sensor
	DSensor(17, M_PI*7/4, std::pair<inches,inches>{8,6})   //right corner distance sensor
);
//BTW, I HAVE NO IDEA HOW THE ANGLE MEASUREMENTS ARE CALCULATED. FIGURE OUT BY EXPERIMENTATION


OdometryComputer Odom = OdometryComputer(Owheels);
//*/

bool balltransferstate = false;

linetracker bottom(ADIAnalogIn({7,'G'}),2800);
linetracker top(ADIAnalogIn('D'),2800);
linetracker middle(ADIAnalogIn({7,'H'}),2800);
linetracker eject(ADIAnalogIn('E'),2300);
Optical input(8);

bool toggleglobaldrive = true;

Motor Lintake(9);
Motor Rintake(19,true);
Motor Shooter(20,true);
Motor Ejector(14);


intakecontroller inta(Lintake,Rintake,Ejector,bottom);
scorecontroller scra(Ejector,Shooter,top);

unifiedliftcontroller ttt(input, {top,middle,bottom}, eject, inta, scra);
