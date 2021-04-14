#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
using namespace pros;
std::uint32_t now = pros::millis();
//DEPRECIATED, SORTA - BELOW IS OLD FUNCTION BASED BALL SORTING SYSTEM
///*

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
	}

	void hack_poop(command a){
		if (a.allow_ejection == true) {
			Shooter.move_velocity(-200);
			Ejector.move_velocity(-200);
		}
	}

		//cancelation case 2: ball present in middle(going to top will cause bottom ball to get stuck in middle)
	/*
		if (balltransferstate && middle.returnval() && bottom.returnval()){
			balltransferstate = false;
			Ejector.move_velocity(0);
			Shooter.move_velocity(0);
		}//
	}*/

	//OG 106 path, lacks new drive features
	/*
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
				new anglereset({0,10}),
				new rotation({30,40,M_PI/2}),
				new intake({65,100},{1,2000}),
				new rotation({95,100,6.22})
			}},
			{position({25.75,61.15,6.22}),{
				new intake({0,100},{1,4200}),
			}},
			{position({25.75,61.15,6.22}),{
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

	    {position({21.5, 48.0, 5.76}), {
			}},
	    {
	        position({78.14, 11.27, 5.62}),    {
	            new intake({0, 30}, {1, 2000}),
	            new intake({50, 100}, {1, 1000})
	        }
	    },
	    {
	        position({94.75, 6.2, 5.485}), {
	            new score({95, 100}, {1, 1000})    // goal 3 (bottom right)
	        }
	    },
			{position({84.70,9.90,M_PI*7/4}),{
				new anglereset({0,10}),
				new rotation({50,100,M_PI/2}),
			}},
	    {position({87.63, 52.53, M_PI/2-0.06}), {
	      new intake({55, 100}, {1, 3500}),
			}
	    },
			{
	    position({85.7, 60.17, 0}), {}
	    },
	    {
	        position({92.5, 60.17, 0}), {
	            new score({30, 100}, {1, 1000})    // goal 4 (right middle)
	        }
	    },
			{position({77.2, 85.88, 0}), {
				new anglereset({0,10}),
				new rotation({50,100,M_PI/2}),
			}
			},

	    {position({77.63, 101.6, M_PI/2-0.07}), {
	            new intake({30, 100}, {1, 4000})    // intaking this before to avoid sketcch movements
	        }
	    },
	    {position({99, 115.77, 0.72}), {	// goal 5 (top right)
				new score({90,100},{1,600})
			}},
	    {position({41.92, 102.44, 0.72}), {
				new anglereset({0,10}),
			//	new odomreset({0,10},std::tuple<inches,inches,SMART_radians>{93.3,117.3,M_PI/4},0.07256416339),
				new rotation({20, 100, M_PI/2})
			}},
// safer alternative to the above move/turn
	// {position({80, 108, M_PI/4}), {}},
	// {position({80, 108, M_PI/2}), {}},
	// {position({32, 108, M_PI/2}), {}},
	{
			position({41.6, 114.6, M_PI/2}), {
			new score({80, 100}, {1, 2000})	// goal 6 (top middle)
		}
	},

	{position({26.17, 108.56, M_PI/2}), {
		new anglereset({0,10}),
		new rotation({50,100,M_PI})
	}},
	{
		position({-5.3, 108.56, M_PI}), {
			new rotation({90,100,2.24}),
			new intake({5, 100}, {1, 4000})
		}
	},
	{position({-14.7, 117.2, 2.34}), {
			new score({90, 100}, {1, 1600})	// goal 7 (top left)
		}
	},
	{position({-5.68, 95.5, 2.34}), {
		new anglereset({0,10}),
		new rotation({40,100,M_PI*3/2}),
		}
	},
	{position({-5.15,64.37,M_PI*3/2}),{
		new intake({25,100},{1,3000}),
	}},
	{position({-1.56,60.86,M_PI}),{
	}},

	{position({-10, 61, M_PI}), {
			new score({70, 100}, {1, 1000})	// goal 8 (left middle)
		}
	},
	{position({-2.24, 61, 3.20}), {
		}
	}
};//*/

//test for distance sensors
/*
std::vector<linearmotion> cmdset = {
	{
			position({0, 26, M_PI/2}), {
					new rotation({40, 100, M_PI*5/4}),
			}
	},
	{
		position({16,16,M_PI*5/4}),{
			new useDistanceSensor({-1,120},{BACK_WALL,LEFT_WALL})
		}
	},
	{
		position({24,24,0}),{
		}
	},
	{
		position({112,36,M_PI*7/4}),{
			new coordinatetarget({15,75},std::pair<inches,inches>{72,72})
		}
	},
	{
		position({128,16,M_PI*7/4}),{
			new useDistanceSensor({-1,120},{RIGHT_WALL,BACK_WALL})
		}
	},
	{
		position({108,48,M_PI/2}),{
		}
	},
	{
		position({108,108,M_PI/4}),{
		}
	},
	{
		position({128,128,M_PI/4}),{
			new useDistanceSensor({-1,120},{FRONT_WALL,RIGHT_WALL})
		}
	},
	{
		position({108,108,M_PI}),{
		}
	},
	{
		position({36,108,M_PI*3/4}),{
		}
	},
	{
		position({16,128,M_PI*3/4}),{
			new useDistanceSensor({-1,120},{LEFT_WALL,FRONT_WALL})
		}
	},
	{
		position({36,112,M_PI*3/2}),{
		}
	},
	{
		position({36,36,M_PI*5/4}),{
		}
	},
	{
		position({16,16,M_PI*5/4}),{
			new useDistanceSensor({-1,120},{BACK_WALL,LEFT_WALL})
		}
	},
};
//*/
std::vector<linearmotion> cmdset = {

	//intake goal 1 balls
		{
				position({36, 26, M_PI/2}), {
						new intake({0,80},{1,2000}),
						new rotation({80, 100, M_PI*5/4}),
				}
		},

		//score all at goal 1, intake bottom wall left ball
		{
			position({12.1,12.1,M_PI*5/4}),{
				new useDistanceSensor({40,120},{BACK_WALL,LEFT_WALL}),
				new intake({90,100},{1,5000}),
				new score({96,100},{2,2000})
			}
		},

		//prepare for intaking bottom left wall ball as well as eject
		{
			position({20,20,M_PI*5/4}),{
				new intake({0,80},{-1,5400}),
				new rotation({30,50,M_PI/2-0.2})
			}
		},

		//intake bottom left wall ball, eject goal 1 blue balls
		{
			position({14.7,36,M_PI*5/4}),{
				new ejectionenable({0,50}),
				new coordinatetarget({15,100},std::pair<inches,inches>{0,36}),
				new intake({20,100},{1,2000})
			}
		},

		//prepare movement to goal 2
		{
			position({60,20,M_PI*5/4}),{
				new coordinatetarget({0,30},std::pair<inches,inches>{0,0}),
				new rotation({30,100,5}),
			}
		},

		//score one on goal 2
		{
			position({67.1,13.7,5}),{
				new score({85,100},{1,2000})
			}
		},

		//intake middle goal left ball
		{
			position({48,71,5}),{
				new coordinatetarget({10,90},std::pair<inches,inches>{48,72}),
				new intake({60,100},{1,2000}),
				new rotation({90,100,0})
			}
		},

		//enter middle goal, intake blue ball
		{
			position({62.05,69.8,0}),{
				new intake({0,100},{1,6000}),
			}
		},

		//score red ball at middle goal
		{
			position({62.05,69.8,0}),{
				new score({0,100},{1,3000})
			}
		},

		//leave middle goal, eject blue balls from bot
		{
			position({50,70,0}),{
				new rotation({40,90,M_PI*5/4+0.2}),
				new intake({20,90},{1,1000}),
				new score({93,100},{2,1500})
			}
		},

		//prepare for left wall middle ball intake and goal 4
		{
			position({50,70,M_PI}),{
			}
		},

		//intake left wall middle ball, reach goal 4
		{
			position({14.35,70.8,M_PI}),{
				new useDistanceSensor({60,100},{LEFT_WALL,LEFT_WALL}),
				new intake({0,100},{1,2000}),
			}
		},

		//score goal 4, intake its blue ball
		{
			position({14.35,70.8,M_PI}),{
				new useDistanceSensor({60,100},{LEFT_WALL,LEFT_WALL}),
				new intake({0,100},{1,2000}),
				new score({0,100},{1,3000})
			}
		},

		//exit goal 4, eject blue ball, prepare to intake top wall left ball
		{
			position({37.5,90,M_PI}),{
				new intake({0,20},{-1,1000}),
				new coordinatetarget({0,20},std::pair<inches,inches>{8,71}),
				new rotation({21,80,M_PI/2}),
				new ejectionenable({60,100}),
				new rotation({90,100,M_PI/2})
			}
		},

		//intake top wall left ball
		{
			position({37.5,120,M_PI/2}),{
				new ejectionenable({0,10}),
				new intake({0,100},{1,4000}),
				new rotation({90,100,M_PI*3/4}),
			}
		},

		//prepare to score goal 5
		{
			position({34,110,M_PI*3/4}),{
			}
		},

		//score goal 5, intake one blue ball
		{
			position({12.1,144-12.1,M_PI*3/4}),{
				new useDistanceSensor({40,120},{LEFT_WALL,FRONT_WALL}),
				new score({90,100},{1,2000}),
				new intake({90,100},{1,3000})
			}
		},

		//exit goal 5, prepare to intake left wall top ball, eject goal 5 blue balls
		{
			position({20,114,M_PI/3*4}),{
				new coordinatetarget({0,20},std::pair<inches,inches>{144,144}),
				new rotation({21,89,M_PI/2}),
				new ejectionenable({45,100}),
				new coordinatetarget({90,100},std::pair<inches,inches>{4,108}),
			}
		},

		//intake left wall top ball
		{
			position({13.5,108,M_PI/3*4}),{
				new ejectionenable({0,10}),
				new coordinatetarget({0,100},std::pair<inches,inches>{4,108}),
				new intake({50,100},{1,2000})
			}
		},

		//prepare for goal 6, intake middle goal top ball
		{
			position({71,96,M_PI*3/4}),{
				new coordinatetarget({20,90},std::pair<inches,inches>{72,96}),
				new rotation({90,100,M_PI/2}),
				new intake({60,100},{1,2000})
			}
		},

		//move to goal 6
		{
			position({69.8,129.8,M_PI/2}),{
				new useDistanceSensor({60,100},{FRONT_WALL,FRONT_WALL}),
			}
		},

		//score on goal 6, intake blue ball
		{
			position({69.8,129.8,M_PI/2}),{
				new useDistanceSensor({0,100},{FRONT_WALL,FRONT_WALL}),
				new score({0,100},{2,2000}),
				new intake({0,100},{1,2000})
			}
		},

		//prepare for top wall right ball, eject blue ball
		{
			position({71.5,120,M_PI/2}),{
				new coordinatetarget({0,20},std::pair<inches,inches>{69.8,144}),
				new rotation({21,100,0}),
				new ejectionenable({50,100})
			}
		},

		//intake top wall right ball, prepare for goal 7
		{
			position({108,126.3,0}),{
				new ejectionenable({0,10}), //here to toggle ejectionenable of last operation off in case 100 is not hit, preventing the toggling of the eject bool to false
				new rotation({95,100,M_PI/4}),
				new intake({55,100},{1,3000})
			}
		},

		//move to and score on goal 7
		{
			position({144-12.1,144-12.1,M_PI/4}),{
				new useDistanceSensor({40,120},{FRONT_WALL,RIGHT_WALL}),
				new score({92,100},{1,2000})
			}
		},

		//exist goal 7, prepare for goal 8, intake middle goal right ball
		{
			position({96,72.8,M_PI/4}),{
				new coordinatetarget({0,20},std::pair<inches,inches>{144,144}),
				new coordinatetarget({20,92},std::pair<inches,inches>{96,72}),
				new rotation({92,100,0}),
				new ejectionenable({25,65}),
				new intake({55,100},{1,2000})
			}
		},

		//move to goal 8, intake right wall middle  ball
		{
			position({129.5,73.2,0}),{
				new intake({0,100},{1,3000}),
				new useDistanceSensor({60,100},{RIGHT_WALL,RIGHT_WALL}),
			}
		},

		//score both balls, intake blue ball
		{
			position({129.5,73.2,0}),{
				new useDistanceSensor({0,100},{RIGHT_WALL,RIGHT_WALL}),
				new score({0,100},{2,4000}),
				new intake({0,100},{1,1000})
			}
		},

		//exit goal 8, prepare to intake bottom wall right ball
		{
			position({115,72.8,0}),{
				new intake({0,60},{-1,2000})
			}
		},

		//intake bottom wall right ball, eject goal 8 blue ball, prepare for intaking right wall bottom ball
		{
			position({112,24,M_PI/4}),{
				new ejectionenable({0,40}),
				new coordinatetarget({0,90},std::pair<inches,inches>{112,24}),
				new rotation({90,100,M_PI*1/4}),
				new intake({55,100},{1,2000})
			}
		},

		//intake right wall botttom ball
		{
			position({131.5,38.6,M_PI/4}),{
				new coordinatetarget({0,90},std::pair<inches,inches>{140,38.6}),
				new intake({55,100},{1,2000})
			}
		},

		//move to goal 9, score both balls and intake one blue ball
		{
			position({144-12.3,12.3,M_PI*7/4}),{
				new useDistanceSensor({40,120},{RIGHT_WALL,BACK_WALL}),
				new score({92,100},{2,2000}),
				new intake({92,100},{1,1000})
			}
		},

		//intake last goal 9 blue ball
		{
			position({144-12.3,12.3,M_PI*7/4}),{
				new useDistanceSensor({0,120},{RIGHT_WALL,BACK_WALL}),
				new intake({0,100},{1,3000})
			}
		},

		//exit goal 9
		{
			position({120,24,M_PI*7/4}),{
				new intake({0,50},{-1,5000})
			}
		},
};
//DoubleIMU Tes(3,15);
void autonomous() {
	delay(100);
	input.set_led_pwm(80);

//	while(Tes.is_calibrating()){pros::delay(10);}
	while(LIM.is_calibrating()){pros::delay(10);}
//	Shooter.move_relative(150,200);
//	Lintake.move_relative(300,200); //these deploys are not implemented due to the vibrations screwing with the inertial sensor calibration
//	Rintake.move_relative(300,200);
	//im.reset();
	/*while(im.is_calibrating()){
		locationC = Odom.cycle(locationC); //odom activated to prevent deploy induced tracking issues
		delay(10);
	}*/
	delay(100);
  locationC = std::tuple<inches,inches,SMART_radians>{36,0,M_PI/2}; //REMEMBER: LOCATIONC IS IN A STATIC ADDRESS
  	for(int i = 0; i < cmdset.size(); i++){
  		while(true){
  		//	break;
  			if (master.get_digital_new_press(DIGITAL_UP)) break;
  			locationC = Odom.cycleIMU(locationC,SMART_radians(degrees(double(LIM.get_rotation()*-1.01056196909)+90)));
  			lcd::print(5,"X: %f",locationC.x);
  			lcd::print(6,"Y: %f",locationC.y);
  			lcd::print(7,"R: %f",locationC.angle);
  			ballindexcontroller2();
			/*	ttt.update_state(currentcommand.allow_ejection); //initial ball situation detection and auto sorting behavior
				ttt.determinetargetstates(); //react to lift and intake output power decisions made in above lines
				ttt.intakeballupdate(); //identify intake ball status after accounting for intake power decisions
  	*/		autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
  			lcd::print(4,"MODE: PAUSED_ODOMENABLE");
  			delay(10);
  		}
  		lcd::print(4,"MODE: MOVE");
		//	lcd::print(1,"%f",Tes.get_heading());

  	//	delay(100);
  	//	SMART_radians realangle = SMART_radians(degrees(double(im.get_rotation()*-1.01006196909)+90));
			currentcommand.DSensor_status = {false,LEFT_WALL,LEFT_WALL}; //THIS IS A HACK. IMPLEMENT END OF MOVEMENT SHUTDOWN CALL SYSTEM FOR LINEARMOTION
			locationC = Odom.cycleIMU(locationC,SMART_radians(degrees(double(LIM.get_rotation()*-1.01056196909)+90)));
			currentcommand = cmdset[i].updatecommand(currentcommand,locationC);     //update command state machine to new movement
			locationC = DSodom.updateposition(currentcommand,locationC); //ensures that all length calcs are in right sensor refrences
			currentcommand.lengthcompute(locationC);
  		currentcommand.percentcompute(locationC);
  		autonbase.profileupdate(currentcommand,locationC);
  		currentcommand = cmdset[i].processcommand(currentcommand,&locationC,/*realangle*/double(locationC.angle));      //trigger instant start commands
  		//checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
  		while(!(currentcommand.disttotgt <= 0.8 && fabs(currentcommand.target.angle.findDiff(currentcommand.target.angle, locationC.angle)) <= 0.0872665 && currentcommand.isidle())){
			//	lcd::print(1,"%f",Tes.get_heading());
				locationC = Odom.cycleIMU(locationC,SMART_radians(degrees(double(LIM.get_rotation()*-1.01056196909)+90)));
  			currentcommand.percentcompute(locationC);
  		 // realangle = SMART_radians(degrees(double(im.get_rotation()*-1.01006196909)+90.0));
  			currentcommand = cmdset[i].processcommand(currentcommand,&locationC,/*realangle*/double(locationC.angle));     //update command state machine to new movement
  		//	lcd::print(4,"Len %f", currentcommand.disttotgt);
  		//	lcd::print(0,"CMPL %f", currentcommand.completion);
				locationC = DSodom.updateposition(currentcommand,locationC);
  			autonbase.updatebase(currentcommand, locationC);//update base motor power outputs for current position
  		//	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0); //uncomment to disable movement
		//		ttt.update_state(currentcommand.allow_ejection); //initial ball situation detection and auto sorting behavior
				ballindexcontroller2();
				currentcommand = inta.refresh(currentcommand); //overwrite lift sorting outputs for commands
  			currentcommand = scra.refresh(currentcommand); //set intake output
				hack_poop(currentcommand); //WARNING: POOPING CANNOT BE DONE AT THE SAME TIME AS SHOOTING. THIS IS A CODE ISSUE
		//		ttt.determinetargetstates(); //react to lift and intake output power decisions made in above lines
		//		ttt.intakeballupdate(); //identify intake ball status after accounting for intake power decisions
  			lcd::print(5,"X: %f",locationC.x);
  			lcd::print(6,"Y: %f",locationC.y);
  			lcd::print(7,"R: %f",locationC.angle);
				if (master.get_digital_new_press(DIGITAL_LEFT)) break;
  			delay(10);
  		}
  	}
  	autonbase.base.move_vector_RAW(std::pair<inches,inches>{0,0},0,0);
  }
