#include "gd32f10x.h"   
#include "PID_angle.H"
#include "foc.H"

//Œª÷√ª∑PID
float angle_err=0.0f,angle_last_err=0.0f,angle_out=0.0f,p=0.4f,d=0.0f;
float angle_pid(float now_angle,float tar_angle)
{
	angle_err = now_angle - tar_angle
	;
	angle_out=p*angle_err+d*(angle_err-angle_last_err);
	//angle_control(angle_out);
	angle_out = limit(angle_out,-6,6);
    angle_last_err=angle_err;
    return angle_out;
}

void  angle_clear(void)
{
	angle_err=0;
	angle_last_err=0;
	angle_out=0;
}

