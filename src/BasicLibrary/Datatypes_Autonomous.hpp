//A header guard
#pragma once
//#include "Library.hpp"
#include <stack>

#ifndef DATATYPES_AUTO_HPP
#define DATATYPES_AUTO_HPP

namespace STL_lib{

    enum angletype{
      NO_REF_TGT = 0,
      REF_TGT = 1
    };

    enum actiontype{
        ROTATE_ACTION = 0,          //a rotation actioniterator
        INTAKE_ACTION = 1,          //a intake actioniterator
        SCORE_ACTION = 2,           //a score actioniterator
        POS_ROTATE_ACTION = 3,      //a point targeting actioniterator
        EJECT_ACTION = 4,           //a ball ejection actioniterator
        ODOM_RESET_ACTION = 5,      //a coordinate reset actioniterator
        ODOM_ANG_RESET = 6,         //a angle reset actioniterator
        DSODOM_CONTROL_ACTION = 7,  //a distance sensor odom toggle actioniterator
    };

    enum WALL_TGT{
      LEFT_WALL = 0, //wall left of back wall
      BACK_WALL = 1, //the wall the driver stands behind
      RIGHT_WALL = 2, //wall right of back wall
      FRONT_WALL = 3, //wall at the other end of the driver's spot
    };


    /* WARNING: THIS THING NEEDS TO BE DECLARED IN RUNTIME AGAIN. PLS USE TEMP VARIABLES OBTAINED FROM THE SOURCE
       Polymorphic parent class which enables users to input a variety of commands with a start threshold.
       While we have a ending interval, it is only used in the rotaiton
     */
    struct actioniterator{
        /******************************************************************************/
        //Internal Variables
        std::vector<double> config; //index 0: start percentage of movement, other indexes vary.
        actiontype type; //variable used to determine data type
        bool hasbeentriggered = false; //automated procdures will act on the last given command; no need to keep spamming it
        bool iscnstupdatetype;

        /******************************************************************************/
        //Constructor(s)
        actioniterator(std::vector<double> val, actiontype typ, bool refreshininterval):config(val),type(typ),iscnstupdatetype(refreshininterval){};

        /******************************************************************************/
        //Main Functions
        /* action behavior
             Should return a void* of whatever datatype the target controller uses. These will be cast to their correct forms
             via the actiontype enum for processing.
         */
        virtual void* getval() {return nullptr;}//behavior function


        /* idle action behavior
             Should return a void* of whatever datatype the target controller uses. These will be cast to their correct forms
             via the actiontype enum for processing. This one is used to reset the controller to it's default format.
             If a definition of this function is not created in a derivived class, the value will persist until it gets overwritten
         */
        virtual void* getdefaultval() {return nullptr;}//behavior function


        /* interval range checker
            Determines if this action has passed it's start threshold. Will be reactivated upon returning to the interval
            Returns the command in a void* pointer which needs to be casted to the right datatype. Determine this with actiontype type.
            If the command does not need to be sent, returns nullptr. This does not tell you if a command has been sent or not.
            In order to determine the completion status of of the action, check the hasbeentriggered boolean.
         */
        void* iterate(percent perc){
          if (!iscnstupdatetype){
            if (perc >= config[0] && !hasbeentriggered && perc <= config[1]){
                hasbeentriggered = true;
                return getval();
            }
            if (perc >= config[1] && hasbeentriggered){
                hasbeentriggered = false;
                return getdefaultval();
            }
          } else {
            if (perc >= config[0] && perc <= config[1]){
                return getval();
            }
          }
          return nullptr; //NULL is kinda jank as it's technically also 0, the int. Due to that we use nullptr
        }
    };


    /* rotation actioniterator
       This is a derivived actioniterator meant for storing rotation control commands
       Alongside the start and end interval, you must implement an angle target for the interval
    */
    struct rotation: public actioniterator{
        SMART_radians angle;
        /******************************************************************************/
        //Constructor(s)
        //Using one vector for all params is inconsistent. TBD: Convert into standard interval vector + command specific params format.
        rotation(std::vector<double> values):actioniterator(values,ROTATE_ACTION,false),angle(values[2]){}


        /******************************************************************************/
        //Primary function(s)
        //Returns a double with the target orientation, must be cast to double then converted to smart radian
        virtual void* getval(){
            return &angle;
        }
    };


    /* intake actioniterator
       This is a derivived actioniterator meant for storing intake control commands
       Alongside the start and end interval, you must append a ball count, as well as a time limit
    */
    struct intake: public actioniterator{
        std::tuple<int,double> targets;
        /******************************************************************************/
        //Constructor(s)
        intake(std::vector<double> values, std::tuple<int,double> tmp):actioniterator(values,INTAKE_ACTION,false),targets(tmp){}


        /******************************************************************************/
        //Primary function(s)
        //Returns a std::tuple<int,double> with the ball count and time limit, must be cast later
        //NOTE THAT THIS MUST BE DELETED UPON OBTAINING THE VALUES
        virtual void* getval(){
            return new std::tuple<int,double>(targets); //we are forced to copy here due to defaultval not working otherwise as it must be blank.
            //returning a void pointer to some random memory location is undefined behavior and won't be all zeros, so we have to do this to wipe that area clean
        }

        //returns targets of zero if outside threshold
        virtual void* getdefaultval(){
            return new std::tuple<int,double>{0,0.0};
        }
    };


    /* score actioniterator
       This is a derivived actioniterator meant for storing score control commands
       Alongside the start and end interval, you must append a ball count, as well as a time limit
    */
    struct score: public actioniterator{
          std::tuple<int,double> targets;
          /******************************************************************************/
          //Constructor(s)
          score(std::vector<double> values, std::tuple<int,double> tg):actioniterator(values,SCORE_ACTION,false),targets(tg){}


          /******************************************************************************/
          //Primary function(s)
          //Returns a std::tuple<int,double> with the ball count and time limit, must be cast later
          //NOTE THAT THIS MUST BE DELETED UPON OBTAINING THE VALUES
          virtual void* getval(){
              return new std::tuple<int,double>(targets);
          }

          //returns targets of zero if outside threshold
          virtual void* getdefaultval(){
              return new std::tuple<int,double>{0,0.0};
          }
      };

      /* useDSensor actioniterator
         This is a derivived actioniterator meant for configuring and enabling DSensorComputer
         Specify the walls which each sensor is touching. Start from the leftmost sensor
      */
      struct useDistanceSensor: public actioniterator{
            std::tuple<WALL_TGT,WALL_TGT> walls;
            /******************************************************************************/
            //Constructor(s)
            useDistanceSensor(std::vector<double> vals, std::tuple<WALL_TGT,WALL_TGT> tg):actioniterator(vals,DSODOM_CONTROL_ACTION,false),walls(tg){}


            /******************************************************************************/
            //Primary function(s)
            //Returns a std::tuple<int,double> with the ball count and time limit, must be cast later
            //NOTE THAT THIS MUST BE DELETED UPON OBTAINING THE VALUES
            virtual void* getval(){
                return new std::tuple<bool,WALL_TGT,WALL_TGT>{true,std::get<0>(walls),std::get<1>(walls)};
            }

            //returns targets of zero if outside threshold
            virtual void* getdefaultval(){
                return new std::tuple<bool,WALL_TGT,WALL_TGT>{false,LEFT_WALL,LEFT_WALL}; //disabled state. walls dont matter b/c toggle is off
            }
        };



    /* coordinatetarget actioniterator
       This is a derivived actioniterator meant for storing score control commands
       Alongside the start and end interval, you must append an x and y targ
    */
    struct coordinatetarget: public actioniterator{
      coordinate set;

      /******************************************************************************/
      //Constructor(s)
      coordinatetarget(std::vector<double> values, coordinate in):actioniterator(values,POS_ROTATE_ACTION,true),set(in){}


      /******************************************************************************/
      //Primary function(s)
      //Returns a coordinate with the target orientation, must be cast to coordinate, and then the relative angle must be determined
      virtual void* getval(){
          return &set; //processing must be done externally as we don't have access to the current position in here, thus we just return our target
      }
  };


    /* ejectionenable  actioniterator
       This is a derivived actioniterator meant for toggling on the ball ejector
       once past the threshold, it will disable the ejector
    */
    struct ejectionenable: public actioniterator{
      /******************************************************************************/
      //Constructor(s)
      ejectionenable(std::vector<double> values):actioniterator(values,EJECT_ACTION,false){}


      /******************************************************************************/
      //Primary function(s)
      //Returns a coordinate with the target orientation, must be cast to coordinate, and then the relative angle must be determined
      virtual void* getval(){
          return new bool(true); //ejectionenable allows pooping for a certain range of positions
      }

      virtual void* getdefaultval(){
          return new bool(false); //disables pooping once outside of interval
      }
  };

  struct odomreset: public actioniterator{
    position hardset;
    radians adjustmentfactor;
    odomreset(std::vector<double> values, position newtpos, radians af):actioniterator(values,ODOM_RESET_ACTION,false),hardset(newtpos),adjustmentfactor(af){}
    virtual void* getval(){
      return new std::tuple<position,radians>{hardset,adjustmentfactor};
    //  return &adjustmentfactor;
    }
  };

  struct anglereset: public actioniterator{
    SMART_radians PLACEHOLD;
    anglereset(std::vector<double> values):actioniterator(values,ODOM_ANG_RESET,false),PLACEHOLD(0){}
    virtual void* getval(){
      return &PLACEHOLD;
    }
  };

  //velocity profile system
  /*
    There are a few restrictions implemented for the sake of practicallity:
       1. The start and end interval weightings are the same - I have no idea when you wouldn't want this so not implemented
       2. S curve profiles will evenly distribute it's three jerk commands during an accelleration - again, why would you not want this behavior
       3. Everything is profiled on a velocity-distance basis, so it won't look like the V-T graphs you would see otherwise
       4. We are taking in distance values. This is due to the fact that timing everything can easly lead to compounding errors from delays in the motors
          Tying everything to physical locations is better as our odometry system means we can error correct in case something messed up
  */
    struct velocityprofile{
      percent accelscalefactor; //effective percentage of velocity profile which is dedicated to accelleration and decelleration
      inches accelerationdistance; //distance to accellerate
      velocityprofile(inches aL, percent aCS):accelerationdistance(aL),accelscalefactor(aCS){};
      virtual percent determinepoweroutput(percent in) = 0;
    };
    //S curve is probably not happening due to conversion from V-T to V-D being a total pain for something this high degree

    //https://www.desmos.com/calculator/muqm8zpo3g. everything is hard constrained to 0-100 as we take percentage inputs
    struct Trapezoidprofile : public velocityprofile{
      double accelcoeff; //coefficient a1 in desmos graph, controls rate of acceleration when speeding up at start
      double decelcoeff; //coefficient a2 in desmos graph, controls rate ot acceleration when slowing down at end of movement
      percent initvel; //parameter v0 in desmos graph, controls initial velocity
      percent endvel; //parameter v1 in desmos graph, controls final velocity. This exists as we go to direct PID close up so we don't actually need the profile to stop us
      Trapezoidprofile(inches l, inches aL, percent init, percent final):velocityprofile(aL,100*(aL/l)),initvel(init),endvel(final){
        accelcoeff = (10000-(initvel*initvel))/(2*accelscalefactor);
        decelcoeff = (10000-(endvel*endvel))/(2*accelscalefactor);
      };
      Trapezoidprofile():velocityprofile(0,0){}; //jank default constructor. Shouldn't be ever used
      percent determinepoweroutput(percent in){
        percent accelcurve = sqrt(2*accelcoeff*in+(initvel*initvel));
        percent decelcurve = sqrt(-2*decelcoeff*(in-100)+(endvel*endvel));
        //NOTE: INFINITE/UNDEFINED FROM ACCELSCALEFAC 0 SHOULD BE RESOLVED VIA THE MIN DIST CHECKS IN BASECONTROLLER_AUTON
        return (percent)std::min<double>(std::min<double>(accelcurve,decelcurve),100.0); //I cant get the tuple version to compile in test environment so we are doing this instead
        //instead of doing the whole check which one should compute now stuff, its easier just to calculate all curves and check which one is the lowest
        //this also prevents edge cases like super short profiles from screwing up. those cases will naturally become triangle profiles.
      }
    };


    struct DSensor{
      pros::Distance sensor;
      SMART_radians orientation; //angle of the sensor relative to the bot X axis(0rad is right)
      coordinate COR_offset; //offset of sensor's measure distance from real center of rotation

      //NOTE: ANGLE IS ANGLE OF LASER LINE RELATIVE TO POSITIVE X AT THE POINT WHERE THE LASER HITS THE WALL
      DSensor(int port, SMART_radians angle, coordinate offset):sensor(port),orientation(angle),COR_offset(offset){}

      //returns raw distance to contact point VERIFIED
      inches returndistance(){
        return inches(millimeter((double)sensor.get())); //we use automatic conversions to get an inches value
      }

      //returns real distance to specified wall NOT VERIFIED
      inches returnadjusteddist(position odomEpos,WALL_TGT Esensedwall){//cannot compute contact wall as needs odom position to calculate
        coordinate tmp = std::pair<inches,inches>{inches(0),returndistance()}; //get distance to wall
        tmp = tmp.transform_matrix(orientation); //rotate to bot-centric refrence frame
        tmp += COR_offset; //adjust for physical offset of sensor from center of rotation
        tmp.self_transform_matrix(odomEpos.angle-M_PI/2); //rotate to global refrence frame
        switch(Esensedwall){
          case LEFT_WALL:
          case RIGHT_WALL: return fabs(tmp.x);
          case BACK_WALL:
          case FRONT_WALL: return fabs(tmp.y);
        }
      }

//VERIFIED
      coordinate return_walldist(position odomEpos,WALL_TGT Esensedwall){
        coordinate tmp = std::pair<inches,inches>{returndistance(),inches(0)}; //get distance to wall
        tmp = tmp.transform_matrix(orientation); //rotate to bot-centric refrence frame
        pros::lcd::print(4,"LOCAL X %f", tmp.x);
        pros::lcd::print(5,"LOCAL Y %f", tmp.y);
        tmp += COR_offset; //adjust for physical offset of sensor from center of rotation
    //    pros::lcd::print(,"COR Local X %f", tmp.x);
        pros::lcd::print(6,"COR Local X %f", tmp.x);
        pros::lcd::print(7,"COR Local Y %f", tmp.y);
        tmp = tmp.transform_matrix(-((odomEpos).angle-M_PI/2)); //rotate to global refrence frame
      //  return tmp;
        switch(Esensedwall){
          //I think the magnitudes are already scaled so that I can just add but not sure so we have the fabs in there to ensure we are within range
          case LEFT_WALL: {return std::pair<inches,inches>{fabs(tmp.x),0};} //left wall distance is distance relative to x = 0
          case RIGHT_WALL: {return std::pair<inches,inches>{144-fabs(tmp.x),0};} //right wall distance is distance relative to x = 144, where we are between 0 and 144
          case BACK_WALL: {return std::pair<inches,inches>{0,fabs(tmp.y)};} //back wall is distance relative to y = 0;
          case FRONT_WALL: {return std::pair<inches,inches>{0,144-fabs(tmp.y)};}
        }
      }

      position Pcorrect(position odomEpos, WALL_TGT Esensedwall){
          //while we could math out which wall we use via old coords, the whole point of this is to not use our old coords
          /*our hard constraints are gonna be more reliable than odometry
          //assuming that our angle is correct, our position constraining system
          //is able to identify our effective distance to wall
          //using this, we are able to get away with using absolute coordinates, as well as on the fly odom calibration*/
          coordinate tmp = return_walldist(odomEpos, Esensedwall);
          if (tmp.x != 0) {odomEpos.x = tmp.x; return odomEpos;}
          odomEpos.y = tmp.y;
          return odomEpos;
      }

    //  coordinates get_adjustment_vector_Pcorrect(position)
    };

    struct command{
        position intentedstartvector = std::tuple<inches,inches,SMART_radians>{0,0,M_PI/2};
        position target; //current target position
        inches length; //length of current processed path
        inches disttotgt; //distance to target
        bool allow_ejection = false; //ball eject toggle;
        std::tuple<inches,percent,percent> motionprofile_config; //ideal velocity profile constraints, indexes: accel dist, start power, end power
        std::tuple<short,double> intake_status; //current balls amount to be taken in and time before timeout
        std::tuple<unsigned short,double> score_status;  //current balls amount to be scored and time before timeout
        std::tuple<bool,WALL_TGT,WALL_TGT> DSensor_status; //index 0: activation of distance sensor coord calc, index 1 and 2: walls the sensors are hitting(Left to right)
        percent completion; //current percentage of path completion, used to trigger commands
        command(position tgt, std::tuple<inches,percent,percent> trapconfig, std::tuple<int,double> in, std::tuple<int,double> out):
        target(tgt),
        intake_status(in),
        score_status(out),
        motionprofile_config(trapconfig){};
        void lengthcompute(position current){
          coordinate temp(current, target);
          length = temp.get_length(); //not a source of div by zero
        }
        void percentcompute(position current){
          coordinate temp(current, target);
          disttotgt = temp.get_length();
          completion = percent(100-(100*disttotgt/length)); //percent is from 0(incomplete) to 100(complete)
        }
        //checks if the lift is idle: returns true if idle, false if active
        bool isidle(){
          return !(bool)(std::get<0>(intake_status) + std::get<0>(score_status));
          //while there exists the edge case that one is positive and the other negative which causes a false positive, that cant happen as we are using unsigneds,
          //as well as the fact that there exists no case for negative balls in intake and score status
        }
};

    //this is not very modular. Will have to be rewritten for alternate sensor configurations which can do angle to walls
    //module for modifying odometry coordinates based on command settings
    //TBD: allow for angle constrainment when both facing same wall, as well as constraining only one global axis
    struct DSensorComputer{
      DSensor LC;
      DSensor RC;
      std::stack<coordinate> pastvalues = {};

      DSensorComputer(DSensor L, DSensor R):LC(L),RC(R){}

      position updateposition(command currentcommand, position read){
        if(std::get<0>(currentcommand.DSensor_status) == true){
          coordinate E = LC.return_walldist(read,std::get<1>(currentcommand.DSensor_status));
          if(std::get<1>(currentcommand.DSensor_status) == std::get<2>(currentcommand.DSensor_status)){
            if (E.x == 0){
              E = return_avg(E);
              read.y = E.y;
            } else {read.x = E.x;}
          } else {
            E += RC.return_walldist(read,std::get<2>(currentcommand.DSensor_status));
            E = return_avg(E);
            read.x = E.x;
            read.y = E.y;
          }
        return read;
      }
    }

      coordinate return_avg(coordinate NV){
        pastvalues.push(NV);
        if(pastvalues.size() > 4){
          pastvalues.pop();
        }
        coordinate RV = std::pair<inches,inches>{0,0};
        //low effort hack solution for iterating through a stack. Doesnt matter as we don't care about the memory location of the values anyways
        std::stack<coordinate> tmp = {};
        while(!pastvalues.empty()){
          RV+=pastvalues.top();
          tmp.push(pastvalues.top());
          pastvalues.pop();
        }
        pastvalues = tmp;
        RV.x = RV.x/pastvalues.size();
        RV.y = RV.y/pastvalues.size();
      }
    };


    //updates command each cycle based on new status
    struct linearmotion{
        position vector;
        std::vector<actioniterator*> commands;

        //Standard obvious constructor
        linearmotion(position a, std::vector<actioniterator*> b):vector(a),commands(b){};

        //Tuple constructor to be used implicitly, probably
        linearmotion(std::tuple<position,std::vector<actioniterator*>> set):vector(std::get<0>(set)),commands(std::get<1>(set)){};

        //returns command with updated request parameters for new movement percentage situation
        command processcommand(command initial, position* current, SMART_radians IMU_ANGLE){
            for (actioniterator* cmd : commands){
                void* valptr  = cmd->iterate(initial.completion);
                if(valptr) {
                    switch (cmd->type) {
                        case ROTATE_ACTION:{
                            vector.angle = *static_cast<SMART_radians*>(valptr); //no delete as this directly points to the array in the command
                            break;
                          }
                        case INTAKE_ACTION:{
                            initial.intake_status =* static_cast<std::tuple<int,double>*>(valptr);
                            delete static_cast<std::tuple<int,double>*>(valptr); //delete because this is dynamically allocated as a new object in cmd->iterate
                            break;
                          }
                        case SCORE_ACTION:{
                            initial.score_status =* static_cast<std::tuple<int,double>*>(valptr);
                            delete static_cast<std::tuple<int,double>*>(valptr); //delete because this is dynamically allocated as a new object in cmd->iterate
                            break;
                          }
                        case POS_ROTATE_ACTION:{
                            coordinate tmp =* static_cast<coordinate*>(valptr); //should be pointer and not copy construct, fix later
                            vector.angle = SMART_radians(atan2(tmp.y - current->y, tmp.x - current->x)); //atan2 returns interval -pi to pi, conversion to SMART_radians adjusts that to 0-2PI standard
                          //  pros::lcd::print(4,"%f, %f, %f", tmp.second-current.y, tmp.first-current.x, vector.angle);
                            break;
                          }
                        case EJECT_ACTION:{
                            initial.allow_ejection =* static_cast<bool*>(valptr);
                            delete static_cast<bool*>(valptr);
                            break;
                          }

                          //this might be broken, check math pls. Its also kinda supersceeded by DSODOM, which directly updates coordinates in its class
                        //NOTE: the radian value is calculated as inaccurate angle - desired value for that angle post reset
                        case ODOM_RESET_ACTION:{
                      //    position temp = *static_cast<position*>(valptr);
                            std::tuple<position,radians> values = *static_cast<std::tuple<position,radians>*>(valptr); //THIS COPIES THE POINTER< IT IS NOT IT
                            coordinate localoffset(*current,initial.target); //make copy
                            position temp = std::get<0>(values);  //make copy
                            temp += localoffset.transform_matrix(IMU_ANGLE-current->angle); //transform coordinate offset from current coord system into corrected coords
                            temp.angle = IMU_ANGLE; //convert current global orientation into new global orientation
                            *current = temp;
                            delete static_cast<std::tuple<position,radians>*>(valptr);
                            break;
                          }

                        case ODOM_ANG_RESET:{
                          current->angle = IMU_ANGLE;
                          break;
                        }

                        case DSODOM_CONTROL_ACTION:{
                          initial.DSensor_status =* static_cast<std::tuple<bool,WALL_TGT,WALL_TGT>*>(valptr);
                          delete static_cast<std::tuple<bool,WALL_TGT,WALL_TGT>*>(valptr);
                        }
                    }
                }
            }
            initial.target.x = vector.x;
            initial.target.y = vector.y;
            initial.target.angle = vector.angle;
            return initial;
        }

        //special function for newly swapped linearmotions
        command updatecommand(command initial, position current){
          initial.intentedstartvector = initial.target;
          initial.target.x = vector.x;
          initial.target.y = vector.y;
          initial.target.angle = vector.angle;
          return initial;
        }
    };

    struct linetracker{
      pros::ADIAnalogIn port;
      bool isactive = false;
      int threshold;
      linetracker(pros::ADIAnalogIn p, int tr):port(p),threshold(tr){};
      //return true if under threshold
      bool returnval(){
        if (port.get_value() <= threshold) return true;
        return false;
      }

      //rising edge case
      bool return_new_press(){
        if (isactive != returnval()) {
          isactive = returnval();
          if (isactive) return true;
      }
      return false;
    }
    };


};

#endif
