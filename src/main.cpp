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

bool BBDZ = false;
void intake_control(){
	Lintake.move_velocity(0);
	Rintake.move_velocity(0);
//	Shooter.move_velocity(0);
//	Ejector.move_velocity(0);
	if (master.get_digital(DIGITAL_L1)){
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
		BBDZ = false;
	}
	if (master.get_digital(DIGITAL_UP)){
		Shooter.move_velocity(-25);
	}
}

polynomial test({0,0,0.00620001});

SMART_radians globalangle = 0;

///*
std::vector<linearmotion> cmdset = {
	  	{
	        position({0, 26, M_PI/2}), {
	            new rotation({40, 100, M_PI*5/4}),
	            new intake({10, 40}, {1, 1000})
	        }
	    },
	    {
	        position({-19, 7, M_PI*5/4}),    {
	            new score({70, 100}, {1, 550})    // goal 1 (bottom left)
	        }
	    },

	    {
	        position({24.5, 5, M_PI*5/4}),    {
	            new rotation({10, 80, M_PI*7/4}),
	            new score({90, 100}, {1, 600})    // goal 2 (bottom middle)
	        }
	    },
			{position({14,60.25,5.7}),{
				new rotation({30,40,M_PI/2}),
				new intake({65,90},{1,1000}),
				new rotation({95,100,0})
			}},
			{position({26.95,60.25,6.22}),{
				new intake({25,100},{1,4200}),
			}},
			{position({26.95,60.25,6.22}),{
				new score({-1,100},{1,600}),
			}},

			{position({14,60.25,6.22}),{
				new anglereset({0,10}),
				new rotation({50,100,M_PI*3/2}),
			}},
	    {position({21.8, 48.35, 5.7}), {
				new score({0,100},{1,1000})
			}},
	    {
	        position({74.3, 8.81, 5.7}),    {
	            new intake({0, 30}, {1, 2000}),
	            new intake({50, 100}, {1, 1000})
	        }
	    },
	    {
	        position({91.15, 1.4, M_PI*7/4}), {
	            new score({90, 100}, {1, 600})    // goal 3 (bottom right)
	        }
	    },

	    {position({88, 58, M_PI*7/4}), {
				new anglereset({0,10}),
				new rotation({10,100,M_PI/2}),
	      new intake({60, 85}, {1, 1000}),
				new rotation({80,100,-0.174533})
	       }
	    },
	    {
	        position({92.5, 58, -0.174533}), {
	            new score({30, 100}, {1, 800})    // goal 4 (right middle)
	        }
	    },
			{position({79.6, 58, -0.174533}), {
				new rotation({50,100,M_PI/2}),
			}
			},

	    {position({82.8, 92, M_PI/2}), {
	            new intake({55, 100}, {1, 1000})    // intaking this before to avoid sketcch movements
	        }
	    },
	    {position({104.42, 110.69, 0.68280}), {	// goal 5 (top right)
				new score({85,100},{1,600})
			}},
	    {position({48.93, 103, M_PI/4}), {
				new anglereset({0,10}),
			//	new odomreset({0,10},std::tuple<inches,inches,SMART_radians>{93.3,117.3,M_PI/4},0.07256416339),
				new rotation({20, 100, M_PI/2})
			}},
// safer alternative to the above move/turn
	// {position({80, 108, M_PI/4}), {}},
	// {position({80, 108, M_PI/2}), {}},
	// {position({32, 108, M_PI/2}), {}},
	{
			position({48.93, 113.75, M_PI/2}), {
			new score({60, 100}, {1, 600})	// goal 6 (top middle)
		}
	},

	{position({32, 110.5, M_PI/2}), {
		new rotation({50,100,M_PI})
	}},
	{
		position({-0.3, 113.2, M_PI-0.225}), {
			new rotation({70,100,2.24}),
			new intake({10, 70}, {1, 1500})
		}
	},
	{position({-7.7, 125.3, 2.24}), {
			new score({90, 100}, {1, 600})	// goal 7 (top left)
		}
	},
	{position({-8.24, 69.4, 2.24}), {
		new rotation({20,100,4.57}),
			new intake({60, 100}, {1, 1400}),
			new rotation({95,100,3.07})
		}
	},
	{position({-12.9, 69.5, 3.07}), {
			new score({70, 100}, {1, 600})	// goal 8 (left middle)
		}
	},
	{position({-8.24, 69.4, 3.07}), {
		}
	}
};
//*/
/*
std::vector<linearmotion> cmdset = {
  {position({0,26,M_PI/2}),{
		new rotation({40,100,M_PI*5/4}),
		new intake({10,40},{1,1000})
	}},
	{position({-20,5,M_PI*5/4}),{
		new score({70,100},{1,550})
	}},
	{position({12,37.5,M_PI*5/4}),{
		new rotation({20,21,0.0})
	}},
	{position({35,37.5,0}),{
		new intake({40,100},{1,2000}),
		new rotation({80,100,M_PI*3/2})
	}},
	{position({35,9,M_PI*3/2}),{
		new score({70,100},{1,1000})
	}},
	{position({35,14,M_PI*3/2}),{
		new rotation({40,100,0})
	}},
	{position({35,14,0}),{
		new intake({60,100},{1,1000})
	}},
//	{position(),{}}
};
*/
/*
std::vector<linearmotion> cmdset = {
   {position({0,20,M_PI/2}),{}},
 	{position({0,0,M_PI}),{}},
};
*/

Optical teest(1);

void ballindexcontroller(){
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);

	if (bottom.returnval() && !top.returnval() && !balltransferstate){
		balltransferstate = true;
	}
	if (balltransferstate) {
		Ejector.move_velocity(200);
	  Shooter.move_velocity(100);
	}
	if (top.returnval() && balltransferstate){
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

void ADVballindexcontroller(){
	Ejector.move_velocity(0);
	Shooter.move_velocity(0);

	//enable move if top is clear for new ball
	if (bottom.returnval() && !top.returnval() && !balltransferstate && !BBDZ){
		balltransferstate = true;
		timeout = 300;
		if (teest.get_hue() > 200 && teest.get_hue() < 250) isblueball = true;
		else isblueball = false;
	}

	//ball movement
	if (balltransferstate) {
		timeout -= 10;
		Ejector.move_velocity(200);
		if (!isblueball) {
			Shooter.move_velocity(100);
		}
	}

	//cancelation case 1: ball is at top
	if (balltransferstate && top.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}

	if (timeout <= 0 && isblueball) {
		balltransferstate = false;
		BBDZ = true;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}

	if (timeout <= -1000 && isblueball) {
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}
	//cancelation case 2: ball present in middle(going to top will cause bottom ball to get stuck in middle)
/*	if (balltransferstate && middle.returnval() && bottom.returnval()){
		balltransferstate = false;
		Ejector.move_velocity(0);
		Shooter.move_velocity(0);
	}*/
}


//control_loop rotcontrol({new Proportional(70,{100,-100})},{100,-100});
void opcontrol() {
///*
//	teest.set_led_pwm(75);
	while(im.is_calibrating()){pros::delay(10);}
/*	currentcommand.lengthcompute(locationC);
		currentcommand.percentcompute(locationC);
		autonbase.updatebase(currentcommand, locationC);
	while(true){
	  locationC = Odom.cycle(locationC);
		currentcommand.percentcompute(locationC);
		lcd::print(1,"X: %f",locationC.x);
		lcd::print(2,"Y: %f",locationC.y);
		lcd::print(3,"R: %f",locationC.angle);
//	*/
/*
		while(true){
	if(master.get_digital_new_press(DIGITAL_B)) {inertialreset();}
//	if(master.get_digital_new_press(DIGITAL_X)) toggleglobaldrive = !toggleglobaldrive;
	if(master.get_digital_new_press(DIGITAL_A)) balltransferstate = false;
	coordinate currentcontrol = coordinate(std::pair<inches,inches>{inches(master.get_analog(ANALOG_LEFT_X)),inches(master.get_analog(ANALOG_LEFT_Y))});
	if(true) {globalangle = SMART_radians(degrees(double(im.get_rotation()*-1.01056196909)));} //NOTE: NO 90 deg offset b/c this is relative to start pos
	//if(toggleglobaldrive == false) {globalangle = SMART_radians(0.00);}
	currentcontrol.self_transform_polar(-globalangle);
	base.move_vector_RAW_AS_M(currentcontrol,-master.get_analog(ANALOG_RIGHT_X),test);
	ballindexcontroller();
	intake_control();
	pros::delay(10);
}*/
//*/
/*	autonbase.updatebase(currentcommand, locationC);
	pros::delay(10);
	}
*/
///*
delay(100);
locationC = std::tuple<inches,inches,SMART_radians>{0,0,M_PI/2};
	for(int i = 0; i < cmdset.size(); i++){
		while(true){
			break;
			if (master.get_digital_new_press(DIGITAL_UP)) break;
			locationC = Odom.cycle(locationC);
			lcd::print(5,"X: %f",locationC.x);
			lcd::print(6,"Y: %f",locationC.y);
			lcd::print(7,"R: %f",locationC.angle);
			ballindexcontroller();
			autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
			lcd::print(4,"MODE: PAUSED_ODOMENABLE");
			delay(10);
		}
		lcd::print(4,"MODE: MOVE");

	//	delay(100);
		currentcommand = cmdset[i].updatecommand(currentcommand,locationC);     //update command state machine to new movement
		currentcommand.lengthcompute(locationC);
		currentcommand.percentcompute(locationC);
		autonbase.profileupdate(currentcommand,locationC);
		//checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
		while(!(currentcommand.disttotgt <= 0.75 && fabs(currentcommand.target.angle.findDiff(currentcommand.target.angle, locationC.angle)) <= 0.0872665 && currentcommand.isidle())){
			locationC = Odom.cycle(locationC);
			currentcommand.percentcompute(locationC);
			SMART_radians realangle = SMART_radians(degrees(double(im.get_rotation()*-1.01106196909)));
			currentcommand = cmdset[i].processcommand(currentcommand,locationC,realangle);     //update command state machine to new movement
			lcd::print(4,"Len %f", currentcommand.disttotgt);
			lcd::print(0,"CMPL %f", currentcommand.completion);
			autonbase.updatebase(currentcommand, locationC);//update base motor power outputs for current position
		//	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
			currentcommand = inta.refresh(currentcommand);
			ballindexcontroller();
			currentcommand = scra.refresh(currentcommand);
			lcd::print(5,"X: %f",locationC.x);
			lcd::print(6,"Y: %f",locationC.y);
			lcd::print(7,"R: %f",locationC.angle);
			delay(10);
		}
	}
	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0);
	//*/
}
