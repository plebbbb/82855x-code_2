#include "main.h"
#include "Library.hpp"
using namespace pros;
using namespace STL_lib;
//below functions should be done in order, from topmost to bottommost

//tracking wheel radius auto tuning, forwards/backwards wheels only
void O_wheelradius_autotune_FB(Distance left, Distance right, holonomicbase val, OdometryWheels wheel){
  control_loop XL({new Proportional(1,{100,-100})},{100,-100});  //X control loop
  control_loop YL({new Proportional(1,{100,-100})},{100,-100});  //Y control loop
  control_loop R({new Proportional(1,{100,-100})},{100,-100});   //Rotation control loop
  double R_val;
  double L_val;
  while(true){
    inches LD = millimeter(left.get());
    inches RD = millimeter(right.get());
    double UPD = R.update(0,LD-RD);
    if (UPD <= 0.1) {val.move_vector_RAW(std::pair<inches,inches>{0.0,0.0}, UPD, 0); break;}
    val.move_vector_RAW_V(std::pair<inches,inches>{0.0,0.0}, UPD, fabs(UPD));
    delay(10);
  }
  inches Lm = millimeter(left.get());
  inches Rm = millimeter(right.get());
  inches Oavg = (Lm+Rm)/2;
  for (int i = 0; i < 15; i++){ //15 iterations of random movement distances to determine wheel radius
    while(true){
      inches LD = millimeter(left.get());
      inches RD = millimeter(right.get());
      double UPD = R.update(0,LD-RD);
      if (UPD <= 0.1) {val.move_vector_RAW(std::pair<inches,inches>{0.0,0.0}, UPD, 0); break;}
      val.move_vector_RAW_V(std::pair<inches,inches>{0.0,0.0}, UPD, fabs(UPD));
      delay(10);
    }
    wheel.reset();
    inches Le = millimeter(left.get());
    inches Re = millimeter(right.get());
    inches OOavg = (Le+Re)/2;
    double move_distance_ABS = 20*(std::rand()/RAND_MAX)-10; //RNG between -10 and 10 inches of movement
    while(true){
      inches LD = millimeter(left.get());
      inches RD = millimeter(right.get());
      inches av = (Lm+Rm)/2;
      inches URD = R.update(0,LD-RD);
      inches LRD = YL.update(Oavg+move_distance_ABS,av);
      if (URD <= 0.1 && LRD <= 0.1 && fabs(av-LD) <= 0.1){
        L_val += (1/15)*((av-OOavg)/wheel.LEFT.get_radian());
        R_val += (1/15)*((av-OOavg)/wheel.RIGHT.get_radian());
        val.move_vector_RAW_V(std::pair<inches,inches>{0,0},0,0);
        break;
      }
      val.move_vector_RAW_V(std::pair<inches,inches>{0,LRD},URD,std::min<double>(10,LRD+URD));
      delay(10);
    }
  }
};
