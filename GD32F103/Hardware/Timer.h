#ifndef _TIMIR_H_
#define _TIMIR_H_
#include <stdint.h>	
#include "gd32f10x.h"    
#include "GPIO.h"
#include "usart.h"
#include "FOC.h"

extern float angle;
void Timer2_init(int periodMS);
 
#endif