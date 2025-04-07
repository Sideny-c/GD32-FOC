#ifndef __I2C_H__
#define __I2C_H__

#include "stdlib.h"
#include "systick.h"
#include "gd32f10x.h" 

#define AS5600_SCLK_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_5)//SCL
#define AS5600_SCLK_Set() gpio_bit_set(GPIOB,GPIO_PIN_5)

#define AS5600_SDIN_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_3)//DIN
#define AS5600_SDIN_Set() gpio_bit_set(GPIOB,GPIO_PIN_3)

#define SDA_IN()  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3)
#define SDA_OUT() gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3)
#define READ_SDA  gpio_input_bit_get(GPIOB,GPIO_PIN_3)

#define AS5600_CMD  0	//Ð´ÃüÁî
#define AS5600_DATA 1	//Ð´Êý¾Ý

void IIC_Start_AS5600();
void IIC_Stop_AS5600();
uint8_t IIC_Wait_Ack_AS5600();
void IIC_Send_Byte_AS5600(uint8_t txd);
uint8_t IIC_Read_Byte_AS5600(unsigned char ack);
void IIC_Ack_AS5600(void);
void IIC_NAck_AS5600(void);


#endif
