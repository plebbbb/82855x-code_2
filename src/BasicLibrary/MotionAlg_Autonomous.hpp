#pragma once
//#include "Library.hpp"

#ifndef MOTIONALG_AUTO_HPP
#define MOTIONALG_AUTO_HPP
extern bool balltransferstate; //HACK

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
    bool statechange = false;
    int tmptimer = 400;
    linetracker toop;
    scorecontroller (pros::Motor SI, pros::Motor SSI, linetracker tope):S(SI),SS(SSI),toop(tope){}
    command refresh(command input){
      if (toop.returnval() != lst){ //get_new_press only returns rising edges, we need descending as well to determine when to delay stopping for shots
        statechange = true;
        lst = !lst;
      }
      if(lst == false) {
        if (!balltransferstate && tmptimer >= 0 && std::get<0>(input.score_status) == 1){  //edge case: coast a bit after ball release
          tmptimer-=10;
        }
        if ((tmptimer <= 0 || balltransferstate) && statechange){
          std::get<0>(input.score_status)--;
          statechange = false;
        }
      }
      if (std::get<1>(input.score_status) > 0 && std::get<0>(input.score_status) > 0){
        SS.move_velocity(200);
        //S.move_velocity(200);
        std::get<1>(input.score_status) -= 10;
      }
      else {
        lst = false;
        statechange = false;
        completion = true;
        std::get<1>(input.score_status) = 0;
        std::get<0>(input.score_status) = 0;
        tmptimer = 400;
        if (!balltransferstate) SS.move_velocity(0);
      }
      return input;

    }
  };

  enum BALL_COLOR{
    EMPTY = 0,
    RED = 1,
    BLUE = 2
  };

  struct ball{
    BALL_COLOR color;
    bool istransfer = false;
    ball(BALL_COLOR detect):color(detect){};
  };


  //the array method probably makes revision difficult, but I cant think of any other way to do non-braindead indexing
  struct unifiedliftcontroller{
    pros::Optical intake;
    std::vector<linetracker> set; //sensors from top sensor, downwards
    linetracker eject; //ejection sensor
    std::vector<ball> ballpositionset = {ball(EMPTY),ball(EMPTY),ball(EMPTY),ball(EMPTY)}; //ball positions, lowest index is shooter, highest index is intake
    intakecontroller in;
    scorecontroller score;

    unifiedliftcontroller(pros::Optical op, std::vector<linetracker> st, linetracker eje, intakecontroller s, scorecontroller i):intake(op),set(st),eject(eje),in(s),score(i){};

    //identifies current statuses of balls
    void updateballstatus(){
      int i = 0;
      if (eject.return_new_press() == true) ballpositionset[1] = ball(EMPTY); //this cannot fail, as there is no way for the positions to get messed up by top sensor detection, as this is the only ball which can hit the top
      for (int g = 0; g < 3; g++){ //iterate from top down
        if (set[g].return_new_press() == true){
          for (int y = 0; y < 4; y++){ //iterate ball positions from top down as well, that way each moving ball will be matched with its sensor correctly
            if (ballpositionset[y].color != EMPTY && ballpositionset[y].istransfer == true){
              ballpositionset[g] = ballpositionset[y];
              ballpositionset[y] = ball(EMPTY); //clear out this position. Its position will be updated down the line once next sensor is checked
              ballpositionset[g].istransfer = false; //disable transfer flag. This will be updated if the ball still needs to move in another function further down the chain
              break; //continue to next sensor
            }
          }
        }
      }
    }

    //flags balls for movement based on current motor outputs
    void determinetargetstates(){
      if(score.SS.get_target_velocity() != 0) ballpositionset[0] = ball(EMPTY); //this is not with the rest of updateballstatus so that scorecontroller gets a chance to set new velocities
      if(score.S.get_target_velocity() != 0) {
        if (ballpositionset[1].color != EMPTY) ballpositionset[1].istransfer = true;
        else ballpositionset[1].istransfer = false;
        if (ballpositionset[2].color != EMPTY) ballpositionset[2].istransfer = true;
        else ballpositionset[2].istransfer = false;

      }
      if(in.L.get_target_velocity() > 0) ballpositionset[3].istransfer = true; //it doesnt matter if there is no ball, color detection function will just set color to EMPTY, meaning ball update function won't move it
      //only positive values to factor in
      else if(ballpositionset[3].color == EMPTY) ballpositionset[3].istransfer = false;
    }

    //detects new balls in the intake, and determines its color
    void intakeballupdate(){
      //proximity maxes out at 255(very close), fyi
      if(intake.get_proximity() > 70){
        //NOTE: THIS MUST TAKE PLACE AFTER INTAKECONTROLLER UPDATE AND DETERMINETARGETSTATE SO THAT INTAKES GET SHUT OFF AFTER HITTING BALL QUOTA. THIS PREVENTS THE SENSOR FROM COUNTING UNINTAKED BALLS
        if(ballpositionset[3].istransfer && ballpositionset[3].color == EMPTY){
          ballpositionset[3].color = color_check();
        }
      }
    }

    //intake color checking function
    BALL_COLOR color_check(){
    //  return BLUE; //TESTING ONLY
      if (fabs(intake.get_hue() - 20) < 5) return RED;
      if (fabs(intake.get_hue() - 220) < 15) return BLUE;
      return EMPTY;
    }

    void update_state(bool ejct){
      updateballstatus();
      set_velocities(ejct);
      //determinetargetstates();
      //intakeballupdate();
      /*
      for(int i = 0; i < 4; i++){
        pros::lcd::print(i,"%d %d %d",ballpositionset[i].color, set[i].returnval(), ballpositionset[i].istransfer);
      };
      pros::lcd::print(4, "BOTTOM: %d", score.S.get_target_velocity());
      pros::lcd::print(5, "TOP: %d", score.SS.get_target_velocity());
      pros::lcd::print(6, "LINTAKE: %d", in.L.get_target_velocity());
      pros::lcd::print(7, "EJECT: %d", eject.returnval());
      //*/
    }

    void set_velocities(bool ejct){
      bool tempflag = true;
      score.S.move_velocity(0);
      score.SS.move_velocity(0);
      if(ballpositionset[0].color == EMPTY){
        for(ball v : ballpositionset){
          if (v.color != EMPTY){
            if (v.color == RED){
              score.S.move_velocity(200);
              score.SS.move_velocity(200);
              goto topballtargeted; //this is just a double break statmeent in one, dont kill me
            }
          }
        }
      }
      //technically, break statements are just macroed goto statements, so this isnt as scuffed as it looks
      topballtargeted:
        if(ballpositionset[3].color != EMPTY && ballpositionset[2].color != EMPTY){
          if (ballpositionset[1].color == EMPTY){
              score.S.move_velocity(200);
          } else {
            //emergency edge case to prevent underutilization: if top slot is availiable but lowers are not, shift everything up, regardless of color
              score.S.move_velocity(200);
              score.SS.move_velocity(200); //shooter on to shift ball forwards
          }
        }

      if(ejct){
        for (int i = 1; i < 4; i++){ //this pattern ensures that the next existing ball which will be pooped is pooped
          if (ballpositionset[i].color != EMPTY){
              if(ballpositionset[i].color == RED) return;
              score.S.move_velocity(-200);
              return;
          }
          //special edge case: all
        }
      }
    }
  };
};

#endif
