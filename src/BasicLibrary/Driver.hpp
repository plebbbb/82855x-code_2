//A header guard
#pragma once


#ifndef DRIVER_HPP
#define DRIVER_HPP

namespace STL_lib{
  struct controlscheme;
  struct intakecontroller;
  struct basecontroller;

  //This struct intends to merge analog inputs and digital inputs into a single container
  struct controlscheme{
    std::vector<pros::controller_analog_e_t> BaseAnalogStick;
    std::vector<pros::controller_digital_e_t> BaseModeButtons;
  };


  enum DRIVE_MOTION_REF{
    LOCAL_AXIS = false,
    GLOBAL_AXIS = true,
  };
/*
  struct basecontroller{
    controlscheme* config;
    pros::Controller* ct;
    DRIVE_MOTION_REF ref = LOCAL_AXIS;
    basecontroller(controlscheme set, pros::Controller ctr){config = &set; ct = &ctr;}
    void drive(){
      coordinate drivedir = std::pair<inches,inches>{ct->get_analog(config->BaseAnalogStick[0]),ct->get_analog(config->BaseAnalogStick[1])};
      if (ct->get_digital_new_press(config->BaseModeButtons[0]))
    }

  };
*/

}

#endif
