#include "gd32f10x.h"
#include "usart.h"
#include "stdio.h"
/*
����Usart0������ TX0 PA9 , 
                 RX0 PA10
*/
void usart_config(void)
{
       /*ʹ��ʱ��  */  
       rcu_periph_clock_enable(RCU_USART_GPIO);
       rcu_periph_clock_enable(RCU_UASRT);

       /*����TXΪ����ģʽ ���*/
       gpio_init(USART_GPIO,USART_AF_PP,GPIO_OSPEED_50MHZ,USART_TX_PIN);
       /*����RXΪ����ģʽ ����*/
       gpio_init(USART_GPIO,USART_AF_IN,GPIO_OSPEED_50MHZ,USART_RX_PIN);

       /*���ô��ڲ���
       һ.��λ����
       ��.���ò�����
       ��.����У��λ
       ��.��������λ
       ��.����ֹͣλ
       */
       /*��λ����*/
       usart_deinit(USART);
       /*���ô��ڲ�����*/
       usart_baudrate_set(USART,USART_BAUDRATE);
       /*���ô���У��λ
       USART_PM_NONE : û��У��λ
       USART_PM_EVEN : ��У��
       USART_PM_ODD  : żУ��
       */
       usart_parity_config(USART,USART_PM_NONE);
       /*���ô�������λ    һ������Ϊ��λ*/
       usart_word_length_set(USART,USART_WL_8BIT);
       /*���ô��ڵ�ֹͣλ    ����Ϊһλ*/
       usart_stop_bit_set(USART,USART_STB_1BIT);
       

       /*�򿪴���
       һ.ʹ�ܴ���
       ��.���ô��ڷ���
       ��.���ô��ڽ���
       */
       /*���ô��ڷ���*/
       usart_transmit_config(USART,USART_TRANSMIT_ENABLE);
       /*���ô��ڽ���*/
       usart_receive_config(USART,USART_RECEIVE_ENABLE);
			 /*ʹ�ܴ���*/
       usart_enable(USART);

}

/*���ͺ���    ����һ���ַ�*/
void usart_send_data(uint8_t ucch){
    usart_data_transmit(USART,(uint8_t)ucch);
    while(RESET == usart_flag_get(USART0,USART_FLAG_TBE));
}

/*���ͺ���    ����һ���ַ���*/
void usart_send_String(uint8_t *ucstr){
    while (ucstr && *ucstr){
       usart_send_data(*ucstr++);
    }
}

/*��дprintf����*/
int fputc(int ch,FILE *f){
    usart_data_transmit(USART,(uint8_t)ch);
    while(RESET == usart_flag_get(USART0,USART_FLAG_TBE));
    // �ȴ��������ݻ�������־λ
    return ch;
}
