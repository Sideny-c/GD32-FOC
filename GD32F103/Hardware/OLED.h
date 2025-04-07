#ifndef __OLED_H
#define __OLED_H 

#include "gd32f10x.h"
#include "stdlib.h"	
#define SIZE 				8
#define Max_Column	128
#define Max_Row			64 
#define X_WIDTH 		128
#define Y_WIDTH 		64
//-----------------OLED端口定义---------------- 

#define OLED_SCL_Set() gpio_bit_set(GPIOA,GPIO_PIN_11);//SCL
#define OLED_SCL_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_11)

#define OLED_SDA_Set() gpio_bit_set(GPIOA,GPIO_PIN_12);//DIN
#define OLED_SDA_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_12)
#define OLED_READ_SDA()  gpio_input_bit_get(GPIOA,GPIO_PIN_12)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 		0
#define IIC_NO_ACK		1
#define IIC_ACK				0

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size2);
void OLED_ShowString(unsigned char x,unsigned char y, unsigned char *p);	
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(unsigned char x,unsigned char y,unsigned char no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowFNum(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2);
void OLED_Scroll(void);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
#endif
