/*
 * utils.h
 *
 *  Created on: Nov 21, 2011
 *      Author: sushil
 */

#ifndef FE_UTILS_H_
#define FE_UTILS_H_

#include <stdlib.h>
#include <const.h>

const float pi = 3.14159;
const float twopi = 2.0 * pi;

inline float clamp(float value, float lower, float upper){
	if (value > upper)
		return upper;
	if(value < lower)
		return lower;
	return value;
}

inline float makeAnglePosNeg(float angle)
{
    //printf("makeAnglePosNeg:%f ", angle);
    while (angle > pi)
        angle -= twopi;
    while (angle < -pi)
        angle += twopi;
    //printf("->%f ", angle);
    return angle;
}

inline float differenceBetweenAngles(float angle1, float angle2){
    return makeAnglePosNeg(angle1 - angle2);
}

inline float feet(float x){
	return 0.3048 * x;
}
inline float meters(float x){
	return x;
}

inline float knots(float x) {
	return 0.514444444 * x;
}
inline float feetPerSecondSquared(float x) {
	return 0.3048 * x;
}

inline float degreesPerSecond(float x) {
	return 0.0174532925 * x;
}

inline float tons(float x){
	return x * 907.18474;
}
inline float pounds(float x){
	return x * 0.45359237;
}

inline int randInt(int low, int high) {
	return low + random() % (high - low);
}

inline bool closeTo(float a, float b){
	return (std::abs(a-b) < 0.1);
}

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

inline ptime getCurrentTime(){
	return boost::posix_time::microsec_clock::local_time();
}



#endif /* FE_UTILS_H_ */
