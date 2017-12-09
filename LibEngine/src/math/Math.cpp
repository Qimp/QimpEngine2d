#include "Private.h"

QIMP_BEGIN_NAMESPACE

//--------------------------------------------------
int NextPow2(int a){
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

uint32 ILog2(uint32 v)
{
	uint32 r;
	uint32 shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}

//--------------------------------------------------
float Normalize(float value, float min, float max){
	return Clamp( (value - min) / (max - min), 0.0f, 1.0f);
}

//check for division by zero???
//--------------------------------------------------
float Map(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < EPSILON){
		//ofLogWarning("ofMath") << "ofMap(): avoiding possible divide by zero, check inputMin and inputMax: " << inputMin << " " << inputMax;
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}

}

//--------------------------------------------------
float Dist(float x1, float y1, float x2, float y2) {
	return sqrt(float((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

//--------------------------------------------------
float DistSquared(float x1, float y1, float x2, float y2) {
	return ( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

//--------------------------------------------------
bool InRange(float t, float min, float max) {
	return t>=min && t<=max;
}

//--------------------------------------------------
float RadToDeg(float radians) {
	return radians * (float)RAD_TO_DEG;
}

//--------------------------------------------------
float DegToRad(float degrees) {
	return degrees * (float)DEG_TO_RAD;
}

float Wrap(float value, float from, float to){
	if(from > to){
		swap(from, to);
	}
	float cycle = to - from;
	if(cycle == 0){
		return to;
	}
	return value - cycle * floor((value - from) / cycle);
}

//--------------------------------------------------
float WrapRadians(float angle, float from, float to){
	return Wrap(angle, from, to);
}

float WrapDegrees(float angle, float from, float to){
	return Wrap(angle, from, to);
}

//--------------------------------------------------
float LerpDegrees(float currentAngle, float targetAngle, float pct) {
	return currentAngle + AngleDifferenceDegrees(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float LerpRadians(float currentAngle, float targetAngle, float pct) {
	return currentAngle + AngleDifferenceRadians(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float AngleDifferenceDegrees(float currentAngle, float targetAngle) {
	return WrapDegrees(targetAngle - currentAngle);
}

//--------------------------------------------------
float AngleDifferenceRadians(float currentAngle, float targetAngle) {
	return  WrapRadians(targetAngle - currentAngle);
}

QIMP_END_NAMESPACE