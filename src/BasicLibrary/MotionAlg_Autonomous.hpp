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
    basecontroller_auton(holonomicbase set, control_loop FB, control_loop LR, control_loop CWCCW):base(set),x(FB),y(LR),rot(CWCCW){}
    command updatebase(command input, position current){
      coordinate globalrelative(current,input.target); //returns distance from current to target, which is the correct input type for PID
      //Base movement mode 1: Direct axle movement
      if (input.length <= 2){
        globalrelative.self_transform_polar(SMART_radians(current.angle-M_PI/4));
        //rotate into direct motor axis, which is CCW 45 degrees from the standard orientation, smart radians automatically constrains to right intervals
        double XA = x.update(globalrelative.x,0);
        double YA = y.update(globalrelative.y,0);
        double RA = rot.update(input.target.angle,current.angle);
        base.move_perp_vector_xdrive(XA,YA,RA);
        //as each PID controller controls specific motors, there is no need to set a speed, the PID loops are to do that
      }
      //Base movement mode 2: Combined local axis heading
      else {


      }
    }
  };
};

#endif
