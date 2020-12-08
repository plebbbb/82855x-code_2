#pragma once
//#include "Library.hpp"

#ifndef MOTIONALG_AUTO_HPP
#define MOTIONALG_AUTO_HPP

namespace STL_lib{
  struct basecontroller_auton{
    holonomicbase base;
    basecontroller_auton(holonomicbase set):base(set){}
    command updatebase(command initial){
    //  base.move_vector_RF

    }
  };
};

#endif
