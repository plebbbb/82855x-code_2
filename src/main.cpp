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
//	Shooter.move_velocity(0);
//	Ejector.move_velocity(0);
	if (master.get_digital(DIGITAL_L1)){
		Lintake.move_velocity(200);
		Rintake.move_velocity(200);
	}
	if (master.get_digital(DIGITAL_R1)){
		Shooter.move_velocity(200);
	//	Ejector.move_velocity(200);
	}
	if (master.get_digital(DIGITAL_R2)){
		Ejector.move_velocity(-200);
	}
	if (master.get_digital(DIGITAL_UP)){
		Shooter.move_velocity(-25);
	}
}

polynomial test({0,0,0.00620001});

SMART_radians globalangle = 0;

/*
std::vector<linearmotion> cmdset = {
  {position({0,26,M_PI/2}),{
		new rotation({40,100,M_PI*5/4}),
		new intake({10,40},{1,1000})
	}},
	{position({-19,7,M_PI*5/4}),{
		new score({70,100},{1,550})
	}},
	{position({24.5,5,M_PI*5/4}),{
		new rotation({10,80,5.7}),
		new score({90,100},{1,600})
	}},
	{position({-3.5,23,5.7}),{}},
	{position({16.4, 51.4,5.7}),{}},
	{
		position({73.4,13.4,5.7}),{
		new intake({0,30},{1,2000}),
		new intake({65,100},{1,2000})
	}},
	{
		position({91.7, 0.2, M_PI*7/4}), {
			new score({90, 100}, {1, 600})
		}},{
		position({86, 9.5, M_PI*1/4}), {
		}},

};
*/
std::vector<linearmotion> cmdset = {
  {position({0,26,M_PI/2}),{
		new rotation({40,100,M_PI*5/4}),
		new intake({10,40},{1,1000})
	}},
	{position({-19,7,M_PI*5/4}),{
		new score({70,100},{1,550})
	}},
	{position({24.5,5,M_PI*5/4}),{
		new rotation({10,80,5.7}),
		new score({90,100},{1,600})
	}},
	{position({-3.5,23,5.7}),{}},
	{position({16.4, 51.4,5.7}),{}},
	{
		position({73.4,13.4,5.7}),{
		new intake({0,30},{1,2000}),
		new intake({65,100},{1,2000})
	}},
	{
		position({91.7, 0.2, M_PI*7/4}), {
			new score({90, 100}, {1, 600})
		}},{
		position({86, 9.5, M_PI*1/4}), {
		}},

// std::vector<linearmotion> cmdset = {
//   {position({0,20,M_PI/2}),{new intake({40,100},{1,5000})}},
// 	{position({0,0,M_PI}),{new score({40,100},{1,8000})}},
// };

bool balltransferstate = false;

void ballindexcontroller(){
	if (bottom.returnval() && !top.returnval() && !balltransferstate){
		balltransferstate = true;
	}
	if (balltransferstate) {
		Ejector.move_velocity(200);
		Shooter.move_velocity(100);
	}
	if (balltransferstate && top.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}
}


void ADVballindexcontroller(){
	Ejector.move_velocity(0);
//	Shooter.move_velocity(0);

	//enable move if top is clear for new ball
	if (bottom.returnval() && !top.returnval() && !balltransferstate){
		balltransferstate = true;
	}

	//ball movement
	if (balltransferstate) {
		Ejector.move_velocity(200);
		Shooter.move_velocity(100);
	}

	//cancelation case 1: ball is at top
	if (balltransferstate && top.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}

	//cancelation case 2: ball present in middle(going to top will cause bottom ball to get stuck in middle)
	if (balltransferstate && middle.returnval() && bottom.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}
}


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
		lcd::print(1,"X: %f",locationC.x);
		lcd::print(2,"Y: %f",locationC.y);
		lcd::print(3,"R: %f",locationC.angle);
	if(master.get_digital_new_press(DIGITAL_B)) {inertialreset();}
	if(master.get_digital_new_press(DIGITAL_X)) toggleglobaldrive = !toggleglobaldrive;
	coordinate currentcontrol = coordinate(std::pair<inches,inches>{inches(master.get_analog(ANALOG_LEFT_X)),inches(master.get_analog(ANALOG_LEFT_Y))});
	if(toggleglobaldrive == true) {globalangle = SMART_radians(degrees(double(im.get_rotation()*-1.01056196909)));} //NOTE: NO 90 deg offset b/c this is relative to start pos
	if(toggleglobaldrive == false) {globalangle = SMART_radians(0.00);}
	currentcontrol.self_transform_polar(-globalangle);
	base.move_vector_RAW_AS_M(currentcontrol,-master.get_analog(ANALOG_RIGHT_X),test);
	ADVballindexcontroller();
	intake_control();
	pros::delay(10);
}
//*/
/*	autonbase.updatebase(currentcommand, locationC);
	pros::delay(10);
	}
*/
///*
delay(100);
	for(int i = 0; i < cmdset.size(); i++){
		currentcommand = cmdset[i].updatecommand(currentcommand,locationC);     //update command state machine to new movement
		currentcommand.lengthcompute(locationC);
		currentcommand.percentcompute(locationC);
		autonbase.profileupdate(currentcommand,locationC);
		//checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
		while(!(currentcommand.disttotgt <= 0.75 && fabs(currentcommand.target.angle.findDiff(currentcommand.target.angle, locationC.angle)) <= 0.0872665 && currentcommand.isidle())){
			locationC = Odom.cycle(locationC);
			currentcommand.percentcompute(locationC);
			currentcommand = cmdset[i].processcommand(currentcommand,locationC);     //update command state machine to new movement
			lcd::print(4,"Len %f", currentcommand.disttotgt);
			lcd::print(0,"CMPL %f", currentcommand.completion);
			lcd::print(5,"X: %f",locationC.x);
			lcd::print(6,"Y: %f",locationC.y);
			autonbase.updatebase(currentcommand, locationC);//update base motor power outputs for current position
		//	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
			currentcommand = inta.refresh(currentcommand);
			ADVballindexcontroller();
			currentcommand = scra.refresh(currentcommand);
			delay(10);
		}
	}
	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0);
	//*/
}
