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
    control_loop rot_vectorM;
  //  control_loop transl_horz;
    Trapezoidprofile velocityprofile;
    basecontroller_auton(holonomicbase set, control_loop FB, control_loop LR, control_loop CWCCW, control_loop ROTVM):
    base(set),
    x(FB),
    y(LR),
    rot(CWCCW),
    rot_vectorM(ROTVM){}
  //  transl_horz(RTA){}
/*
    //cheese strat to also determine which side we transform our coords to.
    double line_closestpt(position point, position p1, position p2){
      coordinate intendedln(p1,p2);
      coordinate tnsf(point);
      tnsf = tnsf.transform_matrix(SMART_radians(-atan2(intendedln.y,intendedln.x))); //rotate so that the intended line is the x axis
      return tnsf.y; //y value gives us magnitude of current pos distance to intended line, as well as direction(pos left, neg right)
    }
*/
    void updatebase(command input, position current){
      coordinate globalrelative(current,input.target); //returns distance from current to target, which is the correct input type for PID
      double RVU = input.target.angle.findDiff(current.angle,input.target.angle);

      //Base movement mode 1: Direct axle movement
      if (input.disttotgt <= 5){
        globalrelative = globalrelative.transform_matrix(SMART_radians((current.angle-3*M_PI/4)));
        //rotate into direct motor axis, which is CCW 45 degrees from the standard orientation, smart radians automatically constrains to right intervals
        double XA = x.update(globalrelative.x,0); //correct interval 100 to -100
        double YA = y.update(globalrelative.y,0); //correct interval 100 to -100
        double RA = rot.update(RVU,0); //correct interval 100 to -100

        base.move_perp_vector_xdrive(XA,YA,RA); //this is a hack function that needs to be made into the same format as printf, where we can have indefinite terms in the odd case that we have an non x drive
        //as each PID controller controls specific motors, there is no need to set a speed, the PID loops do that themselves
      }
      //Base movement mode 2: Combined local axis vector + rotation on a Trapezoid speed profile
      else {

        globalrelative = globalrelative.transform_matrix(SMART_radians((current.angle-M_PI/2)));
        //below: heading PID
      //  double translfac = transl_horz.update(line_closestpt(current, input.intentedstartvector, input.target),0);
    //    globalrelative = globalrelative.transform_matrix(SMART_radians(translfac));
        double rotationfactor = rot_vectorM.update(RVU,0); //interval scaled down to 0.5 to -0.5. May need seperate PID from local axis
        percent speed = velocityprofile.determinepoweroutput(input.completion)*(1+fabs(rotationfactor));
    //    pros::lcd::print(1,"TLAV: %f",atan2(globalrelative.y,globalrelative.x));

        //move_vector_RF automatically scales down globalrelative, so no need to process that here
        base.move_vector_RF(globalrelative, rotationfactor, speed);
        //there may be issues with rotation not getting sufficient power. A potential solution is to apply some kinda weighting function on top of the PID output for rotation weight
      }
    }

    void profileupdate(command input, position current){
      velocityprofile = Trapezoidprofile(input.length,std::get<0>(input.motionprofile_config),std::get<1>(input.motionprofile_config),std::get<2>(input.motionprofile_config));
    }

  };

  struct intakecontroller{
    pros::Motor L;
    pros::Motor R;
    pros::Motor S;
    linetracker ba;
    intakecontroller(pros::Motor LI, pros::Motor RI, pros::Motor SI, linetracker bb):L(LI),R(RI),S(SI),ba(bb){}
    command refresh(command input){
      if (ba.return_new_press()) std::get<0>(input.intake_status)--;
      if (std::get<1>(input.intake_status) > 0 && std::get<0>(input.intake_status) > 0){
        L.move_velocity(200);
        R.move_velocity(200);
    //    S.move_velocity(200);
        std::get<1>(input.intake_status) -= 10;
      }
      else {
        std::get<0>(input.intake_status) = 0;
        std::get<1>(input.intake_status) = 0;
        L.move_velocity(0);
        R.move_velocity(0);
    //    S.move_velocity(0);
      }
      return input;

    }
  };

  struct scorecontroller{
    pros::Motor SS;
    pros::Motor S;
    bool completion = false;
    bool lst = false;
    linetracker toop;
    scorecontroller (pros::Motor SI, pros::Motor SSI, linetracker tope):S(SI),SS(SSI),toop(tope){}
    command refresh(command input){
      if (std::get<1>(input.score_status) > 0){
        SS.move_velocity(200);
        //S.move_velocity(200);
        std::get<1>(input.score_status) -= 10;
        if (toop.returnval() != lst){
          if(lst == false) std::get<0>(input.score_status) -= 1;
          lst = !lst;
        }
        if (std::get<0>(input.score_status) <= 0 && !toop.returnval()){
          completion = true;
        }
        if (completion == true && toop.returnval()){
          if (!(SS.get_target_velocity() == 100)) SS.move_velocity(0);
        }
      }
      else {
        lst = false;
        completion = false;
        std::get<0>(input.score_status) = 0;
        SS.move_velocity(0);
      }
      return input;

    }
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
