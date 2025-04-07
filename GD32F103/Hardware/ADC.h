#ifndef _ADC_H
#define _ADC_H

#include "gd32f10x.h"
#include "systick.h"

void resister_adc_init(void);
void gpio_config(void);
void adc_config(void);

uint16_t read_adc0_data(uint8_t adc_channel); // ��ȡADC0ת���Ľ��
float get_photo_r(void);    //��ȡ�����������ֵ
float get_var_r(void);      //��ȡ�ɵ��������ֵ

extern uint16_t AD_Value[3];
void first_get(uint16_t *First_a,uint16_t *First_b);

#endif
