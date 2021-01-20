#include "main.h"
#include "Library.hpp"
using namespace STL_lib;
OdometryWheels odowheel{DeadWheel(1,2,true,2.75,14), DeadWheel(1,2,true,2.75,14), DeadWheel(1,2,true,2.75,14)};
OdometryComputer odocomp(odowheel);
holonomicbase base(
	{motorwheel(pros::Motor(17),15,15)}
);
basecontroller_auton basecontrol(
  base,
  control_loop({new Proportional(1,{100,-100})},{100,-100}),  //X control loop
  control_loop({new Proportional(1,{100,-100})},{100,-100}),  //Y control loop
  control_loop({new Proportional(1,{100,-100})},{100,-100})   //Rotation control loop
);
position current({0,0,M_PI/2});
command currentcommand(current,{0,0,0},{0,0},{0,0});
std::vector<linearmotion> cmdset = {
  {position({0,0,M_PI}),{}}
};
std::uint32_t now = pros::millis();
void autonomous() {
  for(int i = 0; i < cmdset.size(); i++){
    currentcommand = cmdset[i].processcommand(currentcommand,current);     //update command state machine to new movement
    currentcommand.lengthcompute(current);              								   //internal distance system reset for new movement, % reset in while loop

    //checks if within distance tollerance threshold, as well as if the lift is currently idle during that duration
    while(!(currentcommand.disttotgt <= 2 && currentcommand.isidle())){
	    current = odocomp.cycle(current);                    								 //update coordinates
      currentcommand.percentcompute(current);            									 //determine percentage completion
      currentcommand = cmdset[i].processcommand(currentcommand,current);   //process new commands for completion level
      basecontrol.updatebase(currentcommand,current);                      //update base motor power outputs for current position
      pros::Task::delay_until(&now, 10);                                   //function is frozen until 10ms after beginning of last cycle
    }
  }
}
