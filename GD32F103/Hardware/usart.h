#ifndef _USART_H_
#define _USART_H_

#include "gd32f10x.h"
#include "systick.h"

/*����ʱ�ӵĺ궨��*/
#define RCU_USART_GPIO RCU_GPIOA
#define RCU_UASRT RCU_USART0

/*GPIO���ú궨��*/
#define USART_GPIO GPIOA
#define USART_AF_PP GPIO_MODE_AF_PP
#define USART_AF_IN GPIO_MODE_IN_FLOATING
#define USART_TX_PIN GPIO_PIN_9
#define USART_RX_PIN GPIO_PIN_10

/*���ں궨��*/
#define USART USART0
/*���ڲ�����*/
#define USART_BAUDRATE 115200

/*���ڳ�ʼ��*/
void usart_config(void);
/*���ͺ���*/
void usart_send_data(uint8_t ucch);
/*���ͺ���    ����һ���ַ���*/
void usart_send_String(uint8_t *ucstr);

#endif
