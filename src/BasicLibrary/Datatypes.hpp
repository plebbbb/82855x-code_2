//A header guard
#pragma once

#ifndef DATATYPES_HPP
#define DATATYPES_HPP

namespace STL_lib {

	/*
	These wrappers may appear to be overkill when directly using ints and doubles are enough,
	but they help with organization to an extent, and automatically deal out conversions
	*/

	/*Things to fix:
		- The radians, degrees, and SMART_radians pattern is kinda bad.
		  Doesn't allow for conversion from radian to degrees or smart radians to radians
	*/

 static const std::vector<std::vector<double>> Ptriangle = {
		{1},
	  {1,1},
	  {1,2,1},
	  {1,3,3,1},
	  {1,4,6,4,1},
	  {1,5,10,10,5,1},
	  {1,6,15,20,15,6,1},
	  {1,7,21,35,35,21,7,1},
	  {1,8,28,56,70,56,28,8,1},
	  {1,9,36,84,126,126,84,36,9,1}
	};


	/**************************************************************************************************/
	// ROTATIONAL UNITS

	//A radians datatype, which holds a radians value
	struct radians {
		double value; //This holds your angle value, in radians


		/******************************************************************************/
		//Constructors:
		radians() { value = 0; } //default constructor, if you don't make it anything it's 0 by default

		radians(double angle_in_radians) {
			value = angle_in_radians;
		}

		radians operator=(double angle_in_radians) {
			return radians(angle_in_radians);
		}

		/******************************************************************************/
		//Conversion functions
		operator double() {
			return value;
		}

		/******************************************************************************/
		//Manipulation functions
		void operator+=(radians increment) {
			value += increment;
			//    prune();
		}
	};

	/*A smart radian automatically prunes all values to be within the range of 0 - 2PI
	Its a radian, but smart*/
	struct SMART_radians {
		double value; //This holds your angle value, in radians


		/******************************************************************************/
		//Constructors:
		SMART_radians() { value = 0; } //default constructor, if you don't make it anything it's 0 by default

		SMART_radians(double angle_in_radians) {
			value = angle_in_radians;
			prune();
		}

		// When given two smart radians a and b, returns the difference b - a expressed as a radians diff in the range [-PI, PI)
		radians findDiff(SMART_radians a, SMART_radians b) {
			radians diff(b.value);
			diff.value -= a.value;

			if (diff.value >= M_PI) {
				diff.value -= M_PI * 2;
			}
			if (diff.value < M_PI * (-1)) {
				diff.value += M_PI * 2;
			}

			return diff;
		}

		SMART_radians operator=(double angle_in_radians) {
			return SMART_radians(angle_in_radians);
		}


		/******************************************************************************/
		//Utility functions

		void prune() { //constrains the angle values into the range of 0 - 2PI

			//Division by zero correction is to set angle to 0
			if (isnanf(value) || isinff(value)) value = 0;

			//C++ fmod is actually just a remainder function, we have to do this
			//to conform to normal modulus rules, where there cannot be negative values
			value = fmod(fmod(value,2*M_PI)+2*M_PI,2*M_PI);
		//	value = fmod(value,2*M_PI);
		}

		/******************************************************************************/
		//Conversion functions
		operator double() {
			prune();
			return value;
		}


		/******************************************************************************/
		//Manipulation functions
		void operator+=(SMART_radians increment) {
			value += (double)increment;
			prune();
		}

	};


	//A degrees datatype, which holds a degree value
	struct degrees {
		double value; //This holds your angle value, in degrees


		/******************************************************************************/
		//Constructors:
		degrees() { value = 0; } //default constructor, if you don't make it anything it's 0 by default

		degrees(double angleindegrees) {
			value = angleindegrees;
			//  prune();
		}

		degrees operator=(double angleindegrees) {
			return degrees(angleindegrees);
			//prune();
		}


		/******************************************************************************/
		//Utility functions
		void prune() { //constrains the angle values into the range of 0 - 360 degrees
			value = fmod(value, 360);
		}


		/******************************************************************************/
		//Conversion functions
		operator double() {
			return value;
		}

		operator radians() {
			return radians(value * M_PI / 180.0);
		}

		//you must explicitly state that you are converting to a smart radian before you do so
		explicit operator SMART_radians() {
			return SMART_radians(value * M_PI / 180.0);
		}


		/******************************************************************************/
		//Manipulation functions
		void operator+=(degrees increment) {
			value += increment;
			//prune();
		}

	};



	/**************************************************************************************************/
	// LINEAR UNITS

	//inches, I'll implement cm later
	struct inches {
		double value;

		/******************************************************************************/
		//Constructors:
		inches() { value = 0; } //default constructor, if you don't make it anything it's 0 by default

		inches(double distance) {
			value = distance;
		}
		inches operator=(double distance) {
			return inches(distance);
		}

		/******************************************************************************/
		//Conversion functions:
		operator double() {
			return value;
		}
		//it has to be noted that I can't convert an inches to a radians, or a degrees, as that is two layers of implicit conversion
		//you are allowed to do only a single layer of implicit conversion, everything after that must be explicitly stated.
		/*for example,
		  *degrees variable* = (degrees)((double)*an inch variable*) is legal
		  *degrees variable* = (degrees)(*an inch variable*) is also legal, as you implicitly convert the inch into a double
		  *degrees variable* = *an inch variable* is not legal, as there are two conversions
		*/

		/******************************************************************************/
		//Manipulation functions
		void operator+=(inches increment) {
			value += increment;
		}

		void operator-=(inches increment) {
			value -= increment;
		}
	};

	//inches, I'll implement cm later
	struct millimeter {
		double value;

		/******************************************************************************/
		//Constructors:
		millimeter() { value = 0; } //default constructor, if you don't make it anything it's 0 by default

		millimeter(double distance) {
			value = distance;
		}
		millimeter operator=(double distance) {
			return millimeter(distance);
		}

		/******************************************************************************/
		//Conversion functions:
		operator double() {
			return value;
		}

		operator inches(){
			return inches(value/25.4);
		}
		//it has to be noted that I can't convert an inches to a radians, or a degrees, as that is two layers of implicit conversion
		//you are allowed to do only a single layer of implicit conversion, everything after that must be explicitly stated.
		/*for example,
		  *degrees variable* = (degrees)((double)*an inch variable*) is legal
		  *degrees variable* = (degrees)(*an inch variable*) is also legal, as you implicitly convert the inch into a double
		  *degrees variable* = *an inch variable* is not legal, as there are two conversions
		*/

		/******************************************************************************/
		//Manipulation functions
		void operator+=(inches increment) {
			value += increment;
		}

		void operator-=(inches increment) {
			value -= increment;
		}
	};


	/**************************************************************************************************/
	// PROPORTIONAL UNITS

	//percent, a double constrained to always be 0-100
	struct percent {
		double value;	// always 0-100
		double decValue;	//always 0-1

		/******************************************************************************/
		//Constructors:
		percent() { value = 0; decValue = 0; } //default constructor, if you don't make it anything it's 0 by default

		percent(double in) {
			update(in);
		}
		percent operator=(double in) {
			return percent(in);
		}


		/******************************************************************************/
		//Conversion functions:
		operator double() {
			return value;
		}
		//it has to be noted that I can't convert an inches to a radians, or a degrees, as that is two layers of implicit conversion
		//you are allowed to do only a single layer of implicit conversion, everything after that must be explicitly stated.
		/*for example,
		  *degrees variable* = (degrees)((double)*an inch variable*) is legal
		  *degrees variable* = (degrees)(*an inch variable*) is also legal, as you implicitly convert the inch into a double
		  *degrees variable* = *an inch variable* is not legal, as there are two conversions
		*/

		/******************************************************************************/
		//Manipulation functions
		void operator+=(percent increment) {
			update(value + increment);
		}

		void operator-=(percent increment) {
			update(value - increment);
		}

		//get value from 0 to 1
		double getValue() {
			return (decValue);
		}

		//next two funcs get value ranged 0 to n
		double getValue(int n) {
			return (decValue * n);
		}

		double getValue(double n) {
			return (decValue * n);
		}

	private:
		//constrains value of percent to 0-100 and updates value and decValue
		void update(double perc) {
			//if div by zero error, automatically turn it to 100%
			if (isnanf(perc) || isinff(perc)) {
				value = 100;
				decValue = 1;
				return;
			};
			if (perc > 100) {
				perc = 100;
			}
			if (perc < 0) {
				perc = 0;
			}
			value = perc;
			decValue = perc / 100;
		}
	};


	/**************************************************************************************************/
	// COMPOSITE UNITS

	enum AXIS_VALUES {
		X_AXIS = 0,
		Y_AXIS = 1,
		ROTATION = 2
	};

	//standard position datatype, holds an x, y, and an orientation
	struct position {
		inches x; //x coordinate
		inches y; //y coordinate
		SMART_radians angle; //Smart radian to constrain to 0 - 2*PI


		/******************************************************************************/
		//Constructors:
		/*The constructor for positons uses a std::tuple
		  How you format those is like so {x, y, angle}*/
		position(std::tuple<inches, inches, SMART_radians> set) :
			x(std::get<0>(set)),
			y(std::get<1>(set)),
			angle(std::get<2>(set)) {}


		position operator= (std::tuple<inches, inches, SMART_radians> set) {
			return position(set);
		}

		position(std::pair<inches, inches> set) :
			x(std::get<0>(set)),
			y(std::get<1>(set)) {
			angle = 0;
		}

		position() { x = 0; y = 0; angle = 0; } //default constructor assumes you are at origin facing pos x axis

		/******************************************************************************/
		//Conversion functions
		operator std::tuple<inches, inches, SMART_radians>() {
			return { x,y,angle };
		}

		operator std::pair<inches, inches>() {
			return { x,y };
		}

		std::string debug() {
			return "Position: " + std::to_string((int)this) + "\tx=" + std::to_string(x.value) + "\ty=" + std::to_string(y.value) + "\n";
		}

		/******************************************************************************/
		//Manipulation functions
		/*Remember that an conversion from tuples({x,y,angle}) to position can automatically happen
		Using that instead of an actual position type is also perfectly legal*/
		void operator+=(position change) {
			x += change.x;
			y += change.y;
		//	angle += change.angle; //Smart radians automatically deal with out of bound possiblities
		}

		/*REMEMBER TO CONVERT BACK AFTER!!,
		Also, this is a dumb hack that violates the whole purpose of these datatypes*/
		double operator[](AXIS_VALUES index) {
			switch (index) {
			case X_AXIS: return x;
			case Y_AXIS: return y;
			case ROTATION: return angle;
			}
		}
	};

	//Standard coordinate datatype, holds coordinates of arbritrary vector in R2
	struct coordinate {
		inches x;
		inches y;
		inches length;
		coordinate(std::pair<inches, inches> set) :x(set.first), y(set.second) { get_length(); }
		coordinate(std::tuple<inches, inches> set) :x(std::get<0>(set)), y(std::get<1>(set)) { get_length(); }

		//copy constructor
		coordinate operator=(std::pair<inches, inches> set) {
			return coordinate(set);
		}
		coordinate() { x = 0; y = 0; }

		//relative coordinate calculation from two position
		coordinate(position initial, position final):x(final.x - initial.x),y(final.y - initial.y){
			get_length();
		}

		/******************************************************************************/
		//Utility functions

		//We convert our cartesian coordinates into relative
		void self_transform_polar(SMART_radians offset) {
			get_length();
			SMART_radians polar_angle(atan2(y, x) + offset);
			x = inches(length * cos(double(polar_angle)));
			y = inches(length * sin(double(polar_angle)));
		}

		//this, and self transform polar both seem to be broken for some reason
		void self_transform_matrix(SMART_radians offset){
			x = cos(offset)*x + sin(offset)*x;
			y = - sin(offset)*y + cos(offset)*y;
		}

		coordinate transform_matrix(SMART_radians offset){
			inches xe = inches(cos((double)offset)*(double)x + sin((double)offset)*(double)y);
			inches ye = inches(cos((double)offset)*(double)y - sin((double)offset)*(double)x);
			return coordinate(std::pair<inches,inches>{xe,ye});
		}

		//updates length of coordinate
		inches get_length() {
			length = inches(sqrt(x * x + y * y));
			return length;
		}

		/******************************************************************************/
		//Conversion functions
		operator position() {
			return std::pair<inches, inches>{x, y};
		}

	 	operator std::pair<inches, inches>() {
	 		return { x,y };
		}
/*
		std::string debug() {
			const void * tmp = static_cast<const void*>(this);
			std::stringstream ss;
			ss << tmp;
			return "Coordinate: " + ss.str() + "\tx=" + std::to_string(x.value) +
								"\ty=" + std::to_string(y.value) + "\tlength=" + std::to_string(length.value) + "\n";
		}
*/
		/******************************************************************************/
		//Manipulation functions
		/*Remember that an conversion from pairs({x,y}) to coordinate can automatically happen
		Using that instead of an actual coordinate type is also perfectly legal*/
		void operator+=(position change) {
			x += change.x;
			y += change.y;
			get_length();
		}

		void operator-=(position change) {
			x -= change.x;
			y -= change.y;
			get_length();
		}

		inches operator[](AXIS_VALUES index) {
			switch (index) {
			case X_AXIS: return x;
			case Y_AXIS: return y;
			case ROTATION: return 0;
			}
		}


	};


	//standard form n degree polynomial
	class polynomial{
		std::vector<double> expopwr; //lists coefficients starting from x^0 to x^n
		public:
			polynomial(std::vector<double> expo):expopwr(expo){};
			double compute(double x){
				double returnval = 0;
				for (int i = 0; i < expopwr.size(); i++)	returnval += expopwr[i]*pow(x,i);
				return returnval;
			}
	};


		//n degree bezier curve
		struct bezier{
			std::vector<coordinate> set; //lists transform coordinates from start to end
			bezier(std::vector<coordinate> st):set(st){}
			coordinate compute(percent input){
				double T = input.getValue();
				coordinate tmp = std::pair<inches,inches>{0,0};
				int ivi = 0; //inverse of i
				for(int i = set.size()-1; i > -1; i--){
					//expression: binomial coeff * (1-current progression of curve)^(highest term - current term) * current progression of term^current term * current term transform point
					tmp.x+=Ptriangle[set.size()-1][ivi]*pow((1-T),i)*pow(T,ivi)*set[ivi].x;
					tmp.y+=Ptriangle[set.size()-1][ivi]*pow((1-T),i)*pow(T,ivi)*set[ivi].y;
				}
				return tmp;
			}
		};

		//n degree bezier curve wrapper, which includes the derivative of bezier as well
		struct bezierD{
			bezier main;
			bezier slope; //the slope of a bezier curve is the bezier curve of differences between each transform point
			bezierD(std::vector<coordinate> st):main(st),slope(cut_down(st)){}
			//cuts down bezier transform points into transform points for it's derivitive
			std::vector<coordinate> cut_down(std::vector<coordinate> in){
				std::vector<coordinate> inM(in.size()-1);
				for(int i = 0; i < in.size(); i++){
					inM[i] = {in[i+1].x-in[i].x,in[i+1].y-in[i+1].y};
				}
				return inM;
			}
			position compute(percent input){
				position a; //apparently we had a default constructor for this
				a += main.compute(input);
				coordinate ang = slope.compute(input);
				a.angle = SMART_radians(atan2(ang.y,ang.x));
				return a;
			}
		};
	}

#endif
