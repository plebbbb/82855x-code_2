//This is a central HPP file intended to combine all the imports of other HPP files
#include "main.h" //Main.h is the main PROS library. This library is used to operate your motors and sensors directly.
#include <stack>
#include "Datatypes.hpp" //datatypes is a set of structs that defines units so we don't have to do annoying conversions every time
#include "Odometry.hpp" //Odometry.hpp is our Odometry header file that will hold our functions and classes for odometry
#include "Control_loops.hpp" //Header for control loops
#include "Holonomic.hpp" //Holonomic.hpp is our Base control header file that will hold our functions and classes for base control
#include "Datatypes_Autonomous.hpp" //Header for autonomous datatypes
#include "Driver.hpp" //Header for driver controls
#include "MotionAlg_Autonomous.hpp" //Header for autonomous movement algorithms
#include "Config.hpp" //Header for forward declerations (sketchy)
