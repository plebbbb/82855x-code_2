#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;

//GENERAL IMPORTANT THINGS **************************************************/

Controller master(pros::E_CONTROLLER_MASTER);

position locationE({0,0,M_PI/2});


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




//**************************************************************************//

//ODOM THINGS ***************************************************************/

Imu im(16);

OdometryWheels Owheels{
  DeadWheel({3,'A','B'},false,inches(2.75),14.00),
  DeadWheel({3,'G','H'},true,inches(2.75),14.00),
  DeadWheel({3,'C','D'},true,inches(2.75),14.00)
};

OdometryComputer Odom = OdometryComputer(Owheels);




bool toggleglobaldrive = true;
Motor Lintake(8);
Motor Rintake(9,true);
Motor Shooter(6);
Motor Ejector(10,true);
