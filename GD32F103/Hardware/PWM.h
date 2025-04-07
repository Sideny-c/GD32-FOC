#ifndef _PWM_H
#define _PWM_H

#include "gd32f10x.h"

void timer1_pwm_init(uint16_t psr, uint16_t arr, uint16_t pulse);   //初始化timer2的pwm输出参数
void pwm_a_write(uint16_t Compare);
void pwm_b_write(uint16_t Compare);
void pwm_c_write(uint16_t Compare);
void PWM_Stop(void);
int constrain_int(int amt, int low, int high);

#endif
