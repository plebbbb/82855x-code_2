#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;
std::uint32_t now = pros::millis();
void ballindexcontroller2(){
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
std::vector<linearmotion> cmdset = {
	  	{
	        position({0, 26, M_PI/2}), {
	            new rotation({40, 100, M_PI*5/4}),
	            new intake({0, 80}, {1, 5000})
	        }
	    },
	    {
	        position({-19, 7, M_PI*5/4}),    {
	            new score({70, 100}, {1, 1000})    // goal 1 (bottom left)
	        }
	    },

	    {
	        position({27.7, 6.5, M_PI*5/4}),    {
	            new rotation({10, 80, 5.35}),
	            new score({90, 100}, {1, 1000})    // goal 2 (bottom middle)
	        }
	    },
			{position({12.55,61.15,M_PI*7/4}),{
				new rotation({30,40,M_PI/2}),
				new intake({65,100},{1,2000}),
				new rotation({95,100,6.22})
			}},
			{position({25.25,61.15,6.22}),{
				new intake({15,100},{1,4200}),
			}},
			{position({25.25,61.15,6.22}),{
				new score({-1,100},{1,1000}),
			}},

			{position({14,61.15,6.22}),{
				new anglereset({0,10}),
		//		new intake({0,20},{1,1000}),
				new rotation({80,100,M_PI*5/4}),
			}},
			{position({14,60.25,M_PI*5/4}),{
				new score({0,100},{1,2000}),
			}},

	    {position({21.8, 48.35, 5.7}), {
			}},
	    {
	        position({74.3, 8.81, 5.7}),    {
	            new intake({0, 30}, {1, 2000}),
	            new intake({50, 100}, {1, 1000})
	        }
	    },
	    {
	        position({92.65, 6, M_PI*7/4}), {
	            new score({90, 100}, {1, 1000})    // goal 3 (bottom right)
	        }
	    },
			{position({86,10,M_PI*7/4}),{
				new anglereset({0,10}),
				new rotation({50,100,M_PI/2}),
			}},
	    {position({86, 58, M_PI/2}), {
	      new intake({55, 100}, {1, 3500}),
			}
	    },
			{
	    position({79, 58, 0}), {}
	    },
	    {
	        position({91, 60.6, 0}), {
	            new score({30, 100}, {1, 1000})    // goal 4 (right middle)
	        }
	    },
			{position({76, 58, 0}), {
				new rotation({50,100,M_PI/2}),
			}
			},

	    {position({76, 100.8, M_PI/2}), {
	            new intake({55, 100}, {1, 2000})    // intaking this before to avoid sketcch movements
	        }
	    },
	    {position({96.8, 115.5, 0.76}), {	// goal 5 (top right)
				new score({90,100},{1,600})
			}},
	    {position({41, 103, M_PI/4}), {
				new anglereset({0,10}),
			//	new odomreset({0,10},std::tuple<inches,inches,SMART_radians>{93.3,117.3,M_PI/4},0.07256416339),
				new rotation({20, 100, M_PI/2})
			}},
// safer alternative to the above move/turn
	// {position({80, 108, M_PI/4}), {}},
	// {position({80, 108, M_PI/2}), {}},
	// {position({32, 108, M_PI/2}), {}},
	{
			position({41, 113.5, M_PI/2}), {
			new score({60, 100}, {1, 1000})	// goal 6 (top middle)
		}
	},

	{position({32, 111, M_PI/2}), {
		new rotation({50,100,M_PI})
	}},
	{
		position({-6.61, 111, M_PI}), {
			new rotation({70,100,2.24}),
			new intake({5, 100}, {1, 4000})
		}
	},
	{position({-14.6, 123.4, 2.45}), {
			new score({90, 100}, {1, 1600})	// goal 7 (top left)
		}
	},
	{position({-6.76, 90, 2.46}), {
		new rotation({40,100,4.57}),
		}
	},
	{position({-7.18,66,4.57}),{
		new intake({25,100},{1,2000}),
	}},
	{position({-2.18,66.7,3.20}),{
	}},

	{position({-11.4, 66.7, 3.15}), {
			new score({70, 100}, {1, 1000})	// goal 8 (left middle)
		}
	},
	{position({-2.24, 67, 3.20}), {
		}
	}
};
void autonomous() {
	delay(100);
	Shooter.move_relative(150,200);
//	Lintake.move_relative(300,200); //these deploys are not implemented due to the vibrations screwing with the inertial sensor calibration
//	Rintake.move_relative(300,200);
	while(im.is_calibrating()){
		locationC = Odom.cycle(locationC); //odom activated to prevent deploy induced tracking issues
		delay(10);
	}
  locationC = std::tuple<inches,inches,SMART_radians>{0,0,M_PI/2};
  	for(int i = 0; i < cmdset.size(); i++){
  		while(true){
  			break;
  			if (master.get_digital_new_press(DIGITAL_UP)) break;
  			locationC = Odom.cycle(locationC);
  			lcd::print(5,"X: %f",locationC.x);
  			lcd::print(6,"Y: %f",locationC.y);
  			lcd::print(7,"R: %f",locationC.angle);
  			ballindexcontroller2();
  			autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
  			lcd::print(4,"MODE: PAUSED_ODOMENABLE");
  			delay(10);
  		}
  		lcd::print(4,"MODE: MOVE");

  	//	delay(100);
  		SMART_radians realangle = SMART_radians(degrees(double(im.get_rotation()*-1.01106196909)));
  		currentcommand = cmdset[i].updatecommand(currentcommand,locationC);     //update command state machine to new movement
  		currentcommand.lengthcompute(locationC);
  		currentcommand.percentcompute(locationC);
  		autonbase.profileupdate(currentcommand,locationC);
  		currentcommand = cmdset[i].processcommand(currentcommand,locationC,realangle);     //trigger instant start commands
  		//checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
  		while(!(currentcommand.disttotgt <= 0.8 && fabs(currentcommand.target.angle.findDiff(currentcommand.target.angle, locationC.angle)) <= 0.0872665 && currentcommand.isidle())){
  			locationC = Odom.cycle(locationC);
  			currentcommand.percentcompute(locationC);
  		  realangle = SMART_radians(degrees(double(im.get_rotation()*-1.01106196909)));
  			currentcommand = cmdset[i].processcommand(currentcommand,locationC,realangle);     //update command state machine to new movement
  			lcd::print(4,"Len %f", currentcommand.disttotgt);
  			lcd::print(0,"CMPL %f", currentcommand.completion);
  			autonbase.updatebase(currentcommand, locationC);//update base motor power outputs for current position
  		//	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
  			currentcommand = inta.refresh(currentcommand);
  			ballindexcontroller2();
  			currentcommand = scra.refresh(currentcommand);
  			lcd::print(5,"X: %f",locationC.x);
  			lcd::print(6,"Y: %f",locationC.y);
  			lcd::print(7,"R: %f",locationC.angle);
				if (master.get_digital_new_press(DIGITAL_LEFT)) break;
  			delay(10);
  		}
  	}
  	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0);
  }
