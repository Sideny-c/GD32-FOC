#include "PID_speed.H"
#include "foc.H"
#include "gd32f10x.h"   

//ËÙ¶È»·PID
float Err=0,last_err=0,next_err=0,out=0,add=0,P=1,I=0.1;
float speed_pid(float speed,float tar)
{
    Err=tar-speed;
    add=P*(Err-last_err)+I*(Err);
	add=limit(add,-6,6);
    out+=add;
	out = limit(out,-6,6);
    next_err=last_err;
    last_err=Err;
    return out;
}

void  speedd_clear(void)
{
	Err=0;
	last_err=0;
	next_err=0;
	out=0;
	add=0;
}
