#ifndef _KEY_H
#define _KEY_H__
#include "gd32f10x.h"
#include "systick.h"

//¶¨ÒåÄ£¿éºÅ
typedef enum
{
    KEY0=0,
    KEY1=1,
    KEY2=2,
    KEY3=3,
    KEY4=4
} KEYn_e;

#define KEY0p  GPIO_PIN_12
#define KEY1p  GPIO_PIN_13
#define KEY2p  GPIO_PIN_14
#define KEY3p  GPIO_PIN_15
#define KEY4p  GPIO_PIN_8


#define KB1     gpio_input_bit_get(GPIOB,KEY0p)==0//KEY_UP_PORT
#define KB2     gpio_input_bit_get(GPIOB,KEY1p)==0//KEY_DOWN_PORT
#define KB3     gpio_input_bit_get(GPIOB,KEY2p)==0//KEY_LEFT_PORT
#define KB4     gpio_input_bit_get(GPIOB,KEY3p)==0//KEY_RIGHT_PORT
#define KB5     gpio_input_bit_get(GPIOA,KEY4p)==0//KEY_MID_PORT

#define KEY_UP          1
#define KEY_DOWN        2
#define KEY_LEFT        3
#define KEY_RIGHT       4
#define KEY_MID         5

void Key_Init();
uint8_t Key_Get(void);
unsigned char KEY_Read(KEYn_e KEYno);
char Key_Scan(void);
#endif
