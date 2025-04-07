#include "pwm.h"

//���ܣ���ʼ��timer1��pwm�������
//psr: Ԥ��Ƶ
//arr: �Զ���װ��
//pulse: ����/�Ƚ�
void timer1_pwm_init(uint16_t psr, uint16_t arr, uint16_t pulse)
	{
	timer_parameter_struct timer_init_struct;
	timer_oc_parameter_struct timer_oc_init_struct;
	
	// ʱ��Դ��io��pb0�ĳ�ʼ��
	rcu_periph_clock_enable(RCU_TIMER1); //������ʱ����ʱ��
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);
	
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	// ��ʼ��timer1
	timer_deinit(TIMER1);
	timer_init_struct.prescaler = psr;
	timer_init_struct.period = arr;  //�Զ�װ��ֵ
	timer_init_struct.alignedmode = TIMER_COUNTER_EDGE;  //�������
	timer_init_struct.counterdirection = TIMER_COUNTER_UP; //��������
	timer_init(TIMER1, &timer_init_struct);
	
	// PWM�ĳ�ʼ��
//	timer_oc_init_struct.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_oc_init_struct.outputstate = TIMER_CCX_ENABLE;  //ʹ��ͨ��
	timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_oc_init_struct);
	timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_oc_init_struct);
	timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_oc_init_struct);
	
	timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	//timer_channel_output_pulse_value_config(TIMER1,  TIMER_CH_1|TIMER_CH_2|TIMER_CH_3, pulse);
	// ʹ��timer1
	timer_enable(TIMER1);
	
	//nvic_irq_enable(TIMER1_IRQn, 0, 3);		 /* TIMER1�ж����ã���ռ���ȼ�0�������ȼ�3 */
	//timer_interrupt_enable(TIMER1,TIMER_INT_UP);	/* ʹ�ܸ����ж� */
}

/*
void TIMER1_IRQHandler(void)
{
    NVIC_DisableIRQ(EXTI5_9_IRQn);	 
 

    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);     // ���ж�
 
    NVIC_EnableIRQ(EXTI5_9_IRQn);	
 

}
*/
void pwm_a_write(uint16_t Compare)
{
	timer_channel_output_pulse_value_config(TIMER1,  TIMER_CH_1, Compare);
}

void pwm_b_write(uint16_t Compare)
{
	timer_channel_output_pulse_value_config(TIMER1,  TIMER_CH_2, Compare);
}

void pwm_c_write(uint16_t Compare)
{
	timer_channel_output_pulse_value_config(TIMER1,  TIMER_CH_3, Compare);
}

void PWM_Stop(void)
{
	TIMER_CH0CV(TIMER1) = 0;
	TIMER_CH1CV(TIMER1) = 0;
	TIMER_CH2CV(TIMER1) = 0;
	//timer_disable(TIMER1);
}

int constrain_int(int amt, int low, int high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}