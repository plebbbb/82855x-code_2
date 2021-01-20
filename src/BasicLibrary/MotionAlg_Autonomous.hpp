#pragma once
//#include "Library.hpp"

#ifndef MOTIONALG_AUTO_HPP
#define MOTIONALG_AUTO_HPP

namespace STL_lib{
  struct basecontroller_auton{
    holonomicbase base;
    control_loop x;
    control_loop y;
    control_loop rot;
    Trapezoidprofile velocityprofile;
    basecontroller_auton(holonomicbase set, control_loop FB, control_loop LR, control_loop CWCCW):base(set),x(FB),y(LR),rot(CWCCW){}
    void updatebase(command input, position current){
      if (input.completion == 0){
        velocityprofile = Trapezoidprofile(input.length,std::get<0>(input.motionprofile_config),std::get<1>(input.motionprofile_config),std::get<2>(input.motionprofile_config));
      };
      coordinate globalrelative(current,input.target); //returns distance from current to target, which is the correct input type for PID
      //Base movement mode 1: Direct axle movement
      if (input.length <= 2){
        globalrelative.self_transform_polar(SMART_radians(current.angle-M_PI/4));
        //rotate into direct motor axis, which is CCW 45 degrees from the standard orientation, smart radians automatically constrains to right intervals
        double XA = x.update(globalrelative.x,0); //correct interval 100 to -100
        double YA = y.update(globalrelative.y,0); //correct interval 100 to -100
        double RA = rot.update(input.target.angle,current.angle); //correct interval 100 to -100
        base.move_perp_vector_xdrive(XA,YA,RA); //this is a hack function that needs to be made into the same format as printf, where we can have indefinite terms in the odd case that we have an non x drive
        //as each PID controller controls specific motors, there is no need to set a speed, the PID loops do that themselves
      }
      //Base movement mode 2: Combined local axis vector + rotation on a Trapezoid speed profile
      else {
        globalrelative.self_transform_polar(SMART_radians(current.angle-M_PI/2));
        percent speed = velocityprofile.determinepoweroutput(input.completion);
        double rotationfactor = 0.005*rot.update(input.target.angle,current.angle); //interval scaled down to 0.5 to -0.5. May need seperate PID from local axis
        //move_vector_RF automatically scales down globalrelative, so no need to process that here
        base.move_vector_RF(globalrelative, rotationfactor, speed);
        //there may be issues with rotation not getting sufficient power. A potential solution is to apply some kinda weighting function on top of the PID output for rotation weight
      }
    }
  };

  struct intakecontroller{

  };

  struct scorecontroller{

  };

  struct unifiedliftcontroller{
    bool ballpositions[5] = 
    {0,0,0,0,0};
    //intake, bottom roller, pooper slot, hold slot
    void identifyballstatus(){

    }
  };
};

#endif
