#include "gd32f10x.h"
#include "usart.h"
#include "stdio.h"
/*
串口Usart0的引脚 TX0 PA9 , 
                 RX0 PA10
*/
void usart_config(void)
{
       /*使能时钟  */  
       rcu_periph_clock_enable(RCU_USART_GPIO);
       rcu_periph_clock_enable(RCU_UASRT);

       /*配置TX为复用模式 输出*/
       gpio_init(USART_GPIO,USART_AF_PP,GPIO_OSPEED_50MHZ,USART_TX_PIN);
       /*配置RX为复用模式 输入*/
       gpio_init(USART_GPIO,USART_AF_IN,GPIO_OSPEED_50MHZ,USART_RX_PIN);

       /*配置串口参数
       一.复位串口
       二.配置波特率
       三.配置校验位
       四.配置数据位
       五.配置停止位
       */
       /*复位串口*/
       usart_deinit(USART);
       /*配置串口波特率*/
       usart_baudrate_set(USART,USART_BAUDRATE);
       /*配置串口校验位
       USART_PM_NONE : 没有校验位
       USART_PM_EVEN : 奇校验
       USART_PM_ODD  : 偶校验
       */
       usart_parity_config(USART,USART_PM_NONE);
       /*配置串口数据位    一般设置为八位*/
       usart_word_length_set(USART,USART_WL_8BIT);
       /*配置串口的停止位    设置为一位*/
       usart_stop_bit_set(USART,USART_STB_1BIT);
       

       /*打开串口
       一.使能串口
       二.配置串口发送
       三.配置串口接收
       */
       /*配置串口发送*/
       usart_transmit_config(USART,USART_TRANSMIT_ENABLE);
       /*配置串口接收*/
       usart_receive_config(USART,USART_RECEIVE_ENABLE);
			 /*使能串口*/
       usart_enable(USART);

}

/*发送函数    发送一个字符*/
void usart_send_data(uint8_t ucch){
    usart_data_transmit(USART,(uint8_t)ucch);
    while(RESET == usart_flag_get(USART0,USART_FLAG_TBE));
}

/*发送函数    发送一个字符串*/
void usart_send_String(uint8_t *ucstr){
    while (ucstr && *ucstr){
       usart_send_data(*ucstr++);
    }
}

/*重写printf函数*/
int fputc(int ch,FILE *f){
    usart_data_transmit(USART,(uint8_t)ch);
    while(RESET == usart_flag_get(USART0,USART_FLAG_TBE));
    // 等待发送数据缓存区标志位
    return ch;
}
