#ifndef _USART_H_
#define _USART_H_

#include "gd32f10x.h"
#include "systick.h"

/*开启时钟的宏定义*/
#define RCU_USART_GPIO RCU_GPIOA
#define RCU_UASRT RCU_USART0

/*GPIO配置宏定义*/
#define USART_GPIO GPIOA
#define USART_AF_PP GPIO_MODE_AF_PP
#define USART_AF_IN GPIO_MODE_IN_FLOATING
#define USART_TX_PIN GPIO_PIN_9
#define USART_RX_PIN GPIO_PIN_10

/*串口宏定义*/
#define USART USART0
/*串口波特率*/
#define USART_BAUDRATE 115200

/*串口初始化*/
void usart_config(void);
/*发送函数*/
void usart_send_data(uint8_t ucch);
/*发送函数    发送一个字符串*/
void usart_send_String(uint8_t *ucstr);

#endif
