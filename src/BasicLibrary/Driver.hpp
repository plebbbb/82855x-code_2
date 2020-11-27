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
    std::vector<pros::controller_analog_e_t> AnalogSticks;
    std::vector<pros::controller_digital_e_t> Buttons;
  }

}

#endif
