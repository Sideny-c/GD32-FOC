
#ifndef __AS5600_H
#define __AS5600_H
#include "gd32f10x.h"  


uint8_t AS5600_IIC_Read_OneByte(uint8_t deviceaddr,uint8_t readaddr);
float angle_get(void);
float angle_get_all(float angle);
float get_speed(float angle);
float LowPass_Filter(float x);

#endif
