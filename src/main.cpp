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

polynomial test({0,0,0.00620001});

SMART_radians globalangle = 0;
/*
std::vector<linearmotion> cmdset = {
  {position({0,26,M_PI*5/4}),{}},
	{position({-20,6,M_PI*5/4}),{}},
	{position({-4,22,M_PI*5/4}),{}},
	{position({-4, 63, M_PI}),{}},
	{position({-13,63,M_PI}),{}},
	{position({-4,63,M_PI}),{}},
	{position({-4,112,M_PI*3/4}),{}},
	{position({-11,119,M_PI*3/4}),{}}
};
*/
std::vector<linearmotion> cmdset = {
  {position({10,0,M_PI/2}),{new intake({10,50},{3,1000})}},
	{position({0,0,M_PI/2}),{}},
};
//control_loop rotcontrol({new Proportional(70,{100,-100})},{100,-100});
void opcontrol() {
/*
	while(im.is_calibrating()){pros::delay(10);}
	currentcommand.lengthcompute(locationC);
		currentcommand.percentcompute(locationC);
		autonbase.updatebase(currentcommand, locationC);
	while(true){
	  locationC = Odom.cycle(locationC);
		currentcommand.percentcompute(locationC);
		lcd::print(4,"Len %f", currentcommand.disttotgt);
		lcd::print(0,"CMPL %f", currentcommand.completion);
		lcd::print(5,"X: %f",locationC.x);
		lcd::print(6,"Y: %f",locationC.y);
		lcd::print(7,"R: %f",locationC.angle);
	if(master.get_digital_new_press(DIGITAL_B)) {inertialreset();}
	if(master.get_digital_new_press(DIGITAL_X)) toggleglobaldrive = !toggleglobaldrive;
	coordinate currentcontrol = coordinate(std::pair<inches,inches>{inches(master.get_analog(ANALOG_LEFT_X)),inches(master.get_analog(ANALOG_LEFT_Y))});
	if(toggleglobaldrive == true) {globalangle = SMART_radians(degrees(double(im.get_rotation()*-1.01056196909)));} //NOTE: NO 90 deg offset b/c this is relative to start pos
	if(toggleglobaldrive == false) {globalangle = SMART_radians(0.00);}
	currentcontrol.self_transform_polar(-globalangle);
	base.move_vector_RAW_AS_M(currentcontrol,-master.get_analog(ANALOG_RIGHT_X),test);
	intake_control();
	pros::delay(10);
}
//*/
/*	autonbase.updatebase(currentcommand, locationC);
	pros::delay(10);
	}
*/
//Lintake.move_velocity(200);
//Rintake.move_velocity(200);
//Ejector.move_velocity(-200);
///*
delay(100);
	locationC = {0,0,M_PI/2};
	for(int i = 0; i < cmdset.size(); i++){
		currentcommand = cmdset[i].updatecommand(currentcommand,locationC);     //update command state machine to new movement
		currentcommand.lengthcompute(locationC);
		currentcommand.percentcompute(locationC);
		//checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
		while(!(currentcommand.disttotgt <= 0.75 && currentcommand.isidle())){
			lcd::clear();
			locationC = Odom.cycle(locationC);
			currentcommand.percentcompute(locationC);
			currentcommand = cmdset[i].processcommand(currentcommand,locationC);     //update command state machine to new movement
		//	lcd::print(4,"Len %f", currentcommand.disttotgt);
			lcd::print(0,"CMPL %f", currentcommand.completion);
			lcd::print(5,"X: %f",locationC.x);
			lcd::print(6,"Y: %f",locationC.y);
			lcd::print(7,"RT: %f",currentcommand.target.angle);
			autonbase.updatebase(currentcommand, locationC);//update base motor power outputs for current position
			currentcommand = scra.refresh(currentcommand);
			currentcommand = inta.refresh(currentcommand);
			delay(10);
		}
	}
	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0);
	//*/
}
