//A header guard
#pragma once
//#include "Library.hpp"

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

        /******************************************************************************/
        //Constructor(s)
        actioniterator(std::vector<double> val, actiontype typ):config(val),type(typ){};

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
             If a definition of this function is not created in a derivived class, the value will persist
         */
        virtual void* getdefaultval() {return nullptr;}//behavior function


        /* interval range checker
            Determines if this action has passed it's start threshold. Will be reactivated upon returning to the interval
            Returns the command in a void* pointer which needs to be casted to the right datatype. Determine this with actiontype type.
            If the command does not need to be sent, returns nullptr. This does not tell you if a command has been sent or not.
            In order to determine the completion status of of the action, check the hasbeentriggered boolean.
         */
        void* iterate(percent perc){
          if (perc >= config[0] && !hasbeentriggered && perc <= config[1]){
              hasbeentriggered = true;
              return getval();
          }
          if (perc >= config[1] && hasbeentriggered){
              hasbeentriggered = false;
              return getdefaultval();
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
        rotation(std::vector<double> values):actioniterator(values,ROTATE_ACTION),angle(values[2]){}


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
        intake(std::vector<double> values, std::tuple<int,double> tmp):actioniterator(values,INTAKE_ACTION),targets(tmp){}


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
          score(std::vector<double> values):actioniterator(values,SCORE_ACTION){}


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

    /* positonaltarget actioniterator
       This is a derivived actioniterator meant for storing score control commands
       Alongside the start and end interval, you must append an x and y targ
    */
    struct coordinatetarget: public actioniterator{
      coordinate target;

      /******************************************************************************/
      //Constructor(s)
      coordinatetarget(std::vector<double> values):actioniterator(values,ROTATE_ACTION),target({values[2],values[3]}){}


      /******************************************************************************/
      //Primary function(s)
      //Returns a coordinate with the target orientation, must be cast to coordinate, and then the relative angle must be determined
      virtual void* getval(){
          return &target; //processing must be done externally as we don't have access to the current position in here, thus we just return our target
      }
  };

    struct command{
        position target; //current target position
        inches length;
        std::tuple<int,double> intake_status; //current balls amount to be taken in and time before timeout
        std::tuple<int,double> score_status;  //current balls amount to be scored and time before timeout
        percent completion; //current percentage of path completion, used to trigger commands
        command(position tgt, std::tuple<int,double> in, std::tuple<int,double> out):target(tgt),intake_status(in),score_status(out){}
        void lengthcompute(position current){
          coordinate temp(current, target);
          length = temp.get_length();
        }
        void percentcompute(position current){
          coordinate temp(current, target);
          completion = 100*temp.get_length()/length;
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
        command processcommand(command initial, position current){
            for (actioniterator* cmd : commands){
                void* valptr  = cmd->iterate(initial.completion);
                if(valptr) {
                    switch (cmd->type) {
                        case ROTATE_ACTION:
                            vector.angle =* static_cast<double*>(valptr); //no delete as this directly points to the array in the command
                            break;
                        case INTAKE_ACTION:
                            initial.intake_status =* static_cast<std::tuple<int,double>*>(valptr);
                            delete static_cast<std::tuple<int,double>*>(valptr); //delete because this is dynamically allocated as a new object in the cmd->iterate
                            break;
                        case SCORE_ACTION:
                            initial.score_status =* static_cast<std::tuple<int,double>*>(valptr);
                            delete static_cast<std::tuple<int,double>*>(valptr); //delete because this is dynamically allocated as a new object in the cmd->iterate
                            break;
                        case POS_ROTATE_ACTION:
                            coordinate tmp =* static_cast<coordinate*>(valptr); //should be pointer and not copy construct, fix later
                            vector.angle = SMART_radians(atan2(tmp.y-current.y, tmp.x-current.x)); //atan2 returns interval -pi to pi, cast to SMART_radians adjusts that
                    }
                }
            }
            initial.target.x = vector.x;
            initial.target.y = vector.y;
            initial.target.angle = vector.angle;
            return initial;
        }
    };


};

#endif
