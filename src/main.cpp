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
	LIM.reset();
	delay(10);
	while(LIM.is_calibrating()){pros::delay(10);}
}

bool BBDZ = false;
void intake_control(){
	Lintake.move_velocity(0);
	Rintake.move_velocity(0);
//	Shooter.move_velocity(0);
//	Ejector.move_velocity(0);
	if (master.get_digital(DIGITAL_L1)){
	//	BBDZ = true;
	Lintake.move_velocity(200);
	Rintake.move_velocity(200);
	}

	if (master.get_digital(DIGITAL_L2)){
		Shooter.move_velocity(200);
		Ejector.move_velocity(200);
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

void intake_wait_check(){
	if (!bottom.returnval() && BBDZ){
		Lintake.move_velocity(0);
		Rintake.move_velocity(0);
	} else {
		BBDZ = false;
		Lintake.move_velocity(0);
		Rintake.move_velocity(0);
	}
}

polynomial test({0,0.737075,-0.0274916,0.000446275,-0.0000017788}); //https://www.desmos.com/calculator/jdmxo3mfvs
polynomial spinning({0,0.107181,0,0.0000553549});

SMART_radians globalangle = 0;

void ballindexcontroller(){
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);

	if (bottom.returnval() && !balltransferstate){
		balltransferstate = true;
	}
	if (balltransferstate && !top.returnval()) {
		Ejector.move_velocity(200);
		Shooter.move_velocity(100);
	}
	if (balltransferstate && top.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}

	//cancelation case 2: ball present in middle(going to top will cause bottom ball to get stuck in middle)
/*
	if (balltransferstate && middle.returnval() && bottom.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}//*/
}

bool isblueball = false;
int timeout;
///*
void ADVballindexcontroller(){
Ejector.move_velocity(0);
Shooter.move_velocity(0);

if (bottom.returnval() && !balltransferstate){
	balltransferstate = true;
}
if (balltransferstate && (!top.returnval() || !middle.returnval())) {
	Ejector.move_velocity(200);
	Shooter.move_velocity(100);
}
if (balltransferstate && middle.returnval()){
	balltransferstate = false;
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);
}

if (!balltransferstate && top.returnval()){
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);
}

//cancelation case 2: ball present in middle(going to top will cause bottom ball to get stuck in middle)
/*
if (balltransferstate && middle.returnval() && bottom.returnval()){
	balltransferstate = false;
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);
}//*/
	}//*/
void opcontrol() {
//	autonomous();
	/*while(true){
		lcd::print(0,"LEFT ODOMWHEEL: %f", Owheels.LEFT.get_radian());
		lcd::print(1,"RIGHT ODOMWHEEL: %f", Owheels.RIGHT.get_radian());
		lcd::print(2,"REAR ODOMWHEEL: %f", Owheels.BACK.get_radian());
		delay(10);
	}*/
	while(LIM.is_calibrating()){
		delay(10);
	};
delay(100);
	while(true){
/*	lcd::print(0,"BOTTOM: %d", (int)bottom.returnval());
	lcd::print(1,"TOP: %d", (int)top.returnval());
	lcd::print(2,"Intake State: %s", (balltransferstate) ? "TRANSFERING":"IDLE" );
*/
  locationC = Odom.cycle(locationC);
	if(master.get_digital_new_press(DIGITAL_B)) {inertialreset();}
	if(master.get_digital_new_press(DIGITAL_A)) balltransferstate = false;
	coordinate currentcontrol = coordinate(std::pair<inches,inches>{inches(master.get_analog(ANALOG_LEFT_X)),inches(master.get_analog(ANALOG_LEFT_Y))});
	if(true) {globalangle = SMART_radians(degrees(double(LIM.get_rotation()*-1.01056196909)));} //NOTE: NO 90 deg offset b/c this is relative to start pos
//	if(toggleglobaldrive == false) {globalangle = SMART_radians(0.00);}
	//globalangle = SMART_radians(0.00);
	currentcontrol.self_transform_polar(-globalangle);
/*	lcd::print(0,"X: %f", locationC.x);
/*	lcd::print(1,"Y: %f", locationC.y);*/
//	lcd::print(0,"R: %f", locationC.angle);
	base.move_vector_RAW_AS_M(currentcontrol,spinning.compute((double)-master.get_analog(ANALOG_RIGHT_X)),test);
  //ballindexcontroller();
	ttt.update_state((bool)master.get_digital(DIGITAL_DOWN));
	intake_control();
	ttt.determinetargetstates();
	ttt.intakeballupdate();
	pros::delay(10);
};
}
