#include "gd32f10x.h"
#include "PID_I.H"
#include "FOC.H"

//电流环PID
float Err_I=0,last_err_I=0,next_err_I=0,out_I=0,add_I=0,P_I=0.01,I_I=0.001;
float Iq_pid(float speed,float tar)
{
    Err_I=tar-speed;
    add_I=P_I*(Err_I-last_err_I)+I_I*(Err_I);
	add_I=limit(add_I,-6,6);
    out_I+=add_I;
	out_I = limit(out_I,-6,6);
    next_err_I=last_err_I;
    last_err_I=Err_I;
    return out_I;
}

void  speed_clear(void)
{
	Err_I=0;
	last_err_I=0;
	next_err_I=0;
	out_I=0;
	add_I=0;
}

float Err_2=0,last_err_2=0,next_err_2=0,out_2=0,add_2=0,P_2=0.05,I_2=0.005;
float Id_pid(float speed,float tar)
{
    Err_2=tar-speed;
    add_2=P_2*(Err_2-last_err_2)+I_2*(Err_2);
	add_2=limit(add_2,-6,6);
    out_2+=add_2;
	out_2 = limit(out_2,-1,1);
    next_err_2=last_err_2;
    last_err_2=Err_2;
    return out_2;
}

void  speed_2_clear(void)
{
	Err_2=0;
	last_err_2=0;
	next_err_2=0;
	out_2=0;
	add_2=0;
}

//低通滤波器
static float y_prev,y_prev2;
float LowPass_Filter_I(float x,int flag)
{	
	int ret;
	if(flag == 0)
	{
		float y = 0.9*y_prev + 0.1*x;
		y_prev=y;
		ret = y;
	}
	
	if(flag == 1)
	{
		float y2 = 0.9*y_prev2 + 0.1*x;
		y_prev2=y2;
		ret = y2;
	}
	return ret;
}