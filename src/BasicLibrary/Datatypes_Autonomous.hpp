//A header guard
#pragma once
//#include "Library.hpp"

#ifndef DATATYPES_AUTO_HPP
#define DATATYPES_AUTO_HPP

namespace STL_lib{

  enum actiontype{
      ROTATE_ACTION = 0,
      INTAKE_ACTION = 1,
      SCORE_ACTION = 2
  };
  /*

  /* WARNING: THIS THING NEEDS TO BE DECLARED IN RUNTIME AGAIN. PLS USE TEMP VARIABLES OBTAINED FROM THE SOURCE
     Polymorphic parent class which enables users to input a variety of commands with a start threshold.
     End threshold doesn't seem useful, so it is currently removed, but it can be reimplemented again if
     the need arises
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
      void* iterate(double perc){
          if (perc >= config[0] && !hasbeentriggered){
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


  struct rotation: public actioniterator{
      /******************************************************************************/
      //Constructor(s)
      rotation(std::vector<double> values):actioniterator(values,ROTATE_ACTION){}


      /******************************************************************************/
      //Primary function(s)
      //Returns a smart radian with the target orientation, must be cast
      virtual void* getval(){
          return &config[1];
      }
  };


};
