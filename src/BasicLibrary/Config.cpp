#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;

//GENERAL IMPORTANT THINGS **************************************************/

pros::Controller master(pros::E_CONTROLLER_MASTER);

SMART_radians globalangle = 0;


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

pros::Imu im(16);

OdometryWheels Owheels{DeadWheel(1,2,true,2.75,14), DeadWheel(1,2,true,2.75,14), DeadWheel(1,2,true,2.75,14)};

OdometryComputer Odom(Owheels);




bool toggleglobaldrive = true;
Motor Lintake(8);
Motor Rintake(9,true);
Motor Shooter(6);
Motor Ejector(10,true);
