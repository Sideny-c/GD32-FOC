#ifndef __PID_I_H
#define __PID_I_H

float Iq_pid(float speed,float tar);
void  speed_clear(void);

float Id_pid(float speed,float tar);
void  speed_2_clear(void);

float LowPass_Filter_I(float x,int flag);

#endif