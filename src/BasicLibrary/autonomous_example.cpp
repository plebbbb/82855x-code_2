#include "main.h"
#include "BasicLibrary/Library.hpp"
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

int i = 0;
void autonomous() {
  currentcommand.lengthcompute(current);
  for(int i = 0; i < cmdset.size(); i++){
    while(true){
	    odocomp.cycle(current);
      cmdset[i].processcommand(currentcommand,current);
      basecontrol.updatebase(currentcommand,current);
    }
  }
}
