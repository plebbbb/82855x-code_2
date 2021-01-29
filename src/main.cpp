#include "main.h"
#include "BasicLibrary/Library.hpp"
using namespace STL_lib;
using namespace pros;
void initialize() {
	pros::lcd::initialize();
	pros::delay(100);
	printf("test");
}
void disabled() {}
void competition_initialize() {}
void inertialreset(){
	im.reset();
	delay(10);
	while(im.is_calibrating()){pros::delay(10);}
}
void intake_control(){
	Lintake.move_velocity(0);
	Rintake.move_velocity(0);
	Shooter.move_velocity(0);
	Ejector.move_velocity(0);
	if (master.get_digital(DIGITAL_L1)){
		Lintake.move_velocity(200);
		Rintake.move_velocity(200);
	}
	if (master.get_digital(DIGITAL_R1)){
		Shooter.move_velocity(200);
		Ejector.move_velocity(200);
	}
	if (master.get_digital(DIGITAL_R2)){
		Ejector.move_velocity(-200);
	}
	if (master.get_digital(DIGITAL_UP)){
		Shooter.move_velocity(-100);
	}
}


SMART_radians globalangle = 0;

void opcontrol() {
//	while(im.is_calibrating()){pros::delay(10);}
	currentcommand.lengthcompute(locationC);
while(true){
  locationC = Odom.cycle(locationC);
	currentcommand.percentcompute(locationC);
	lcd::print(0,"Len %f", currentcommand.disttotgt);
	lcd::print(5,"X: %f",locationC.x);
	lcd::print(6,"Y: %f",locationC.y);
	lcd::print(7,"R: %f",locationC.angle);//*/
/*	if(master.get_digital_new_press(DIGITAL_B)) {inertialreset();}
	if(master.get_digital_new_press(DIGITAL_X)) toggleglobaldrive = !toggleglobaldrive;
	coordinate currentcontrol = coordinate(std::pair<inches,inches>{inches(master.get_analog(ANALOG_LEFT_X)),inches(master.get_analog(ANALOG_LEFT_Y))});
	if(toggleglobaldrive == true) {globalangle = SMART_radians(degrees(double(im.get_rotation()*-1.01056196909)));} //NOTE: NO 90 deg offset b/c this is relative to start pos
	if(toggleglobaldrive == false) {globalangle = SMART_radians(0.00);}
	currentcontrol.self_transform_polar(-globalangle);
	base.move_vector_RAW_AS(currentcontrol,-master.get_analog(ANALOG_RIGHT_X));
	intake_control();
*/
	autonbase.updatebase(currentcommand, locationC);
	pros::delay(10);
	}
}
