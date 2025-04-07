#include "pwm.h"

//功能：初始化timer1的pwm输出参数
//psr: 预分频
//arr: 自动重装载
//pulse: 捕获/比较
void timer1_pwm_init(uint16_t psr, uint16_t arr, uint16_t pulse)
	{
	timer_parameter_struct timer_init_struct;
	timer_oc_parameter_struct timer_oc_init_struct;
	
	// 时钟源和io口pb0的初始化
	rcu_periph_clock_enable(RCU_TIMER1); //开启定时器的时钟
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);
	
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	// 初始化timer1
	timer_deinit(TIMER1);
	timer_init_struct.prescaler = psr;
	timer_init_struct.period = arr;  //自动装载值
	timer_init_struct.alignedmode = TIMER_COUNTER_EDGE;  //中央对齐
	timer_init_struct.counterdirection = TIMER_COUNTER_UP; //计数方向
	timer_init(TIMER1, &timer_init_struct);
	
	// PWM的初始化
//	timer_oc_init_struct.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_oc_init_struct.outputstate = TIMER_CCX_ENABLE;  //使能通道
	timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_oc_init_struct);
	timer_channel_output_config(TIMER1, TIMER_CH_2, &timer_oc_init_struct);
	timer_channel_output_config(TIMER1, TIMER_CH_3, &timer_oc_init_struct);
	
	timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER1, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	//timer_channel_output_pulse_value_config(TIMER1,  TIMER_CH_1|TIMER_CH_2|TIMER_CH_3, pulse);
	// 使能timer1
	timer_enable(TIMER1);
	
	//nvic_irq_enable(TIMER1_IRQn, 0, 3);		 /* TIMER1中断设置，抢占优先级0，子优先级3 */
	//timer_interrupt_enable(TIMER1,TIMER_INT_UP);	/* 使能更新中断 */
}

/*
void TIMER1_IRQHandler(void)
{
    NVIC_DisableIRQ(EXTI5_9_IRQn);	 
 

    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);     // 清中断
 
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