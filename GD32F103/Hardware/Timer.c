#include "Timer.h"
#include "main.h"
void Timer2_init(int periodMS)
{
    
    timer_parameter_struct timer_initpara; //定时器参数
	  timer_struct_para_init(&timer_initpara);
     // 时钟使能 
    rcu_periph_clock_enable(RCU_TIMER2);  
    timer_deinit(TIMER2);

    /*初始化定时器参数*/
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 720-1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = periodMS-1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER2, &timer_initpara);

   timer_auto_reload_shadow_enable(TIMER2);
   timer_interrupt_flag_clear(TIMER2,TIMER_INT_UP);
   
   /* 设置允许更新中断TIMERx-DMAINTEN*/ 
   timer_interrupt_enable(TIMER2,TIMER_INT_UP); 
  
    /*中断优先级设置*/
    //TIMER1 interrupt setting, preemptive priority 0, sub-priority 3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER2_IRQn, 0, 3); 
    
   /* 使能定时器*/
    timer_enable(TIMER2);   

   // rcu_periph_clock_disable(RCU_TIMER5);/*先关闭等待使用*/ 
}
float a;
/** 中断服务程序
  * @brief  This function handles TIMER1 interrupt request.
  * @param  None
  * @retval None
  */

extern float angle;
void TIMER2_IRQHandler(void)
{
  if ( SET == timer_interrupt_flag_get(TIMER2 , TIMER_INT_UP) ) 
  {
		
		angle = angle_get();
		
		all_angle = angle_get_all(angle);
	
	  //speed=get_speed(all_angle);
		//speed = LowPass_Filter(speed);
		//speed_out = speed_pid(speed,1);
		//setPhaseVoltage(speed_out,0,angle);
		

		local_out = angle_pid(all_angle,30);
		setPhaseVoltage(local_out,0,all_angle); //uq<0 +
		  
		//printf("Ud:%d\n",123);
		
		//setPhaseVoltage_I(i_1,i_2,i_3,angle);
		
				a += 0.005;
				if(a < 3.2)
					gpio_bit_set(GPIOA,GPIO_PIN_15);
				else
					gpio_bit_reset(GPIOA,GPIO_PIN_15);
				if(a > 6.2)
					a = 0;
    timer_interrupt_flag_clear(TIMER2 , TIMER_INT_UP);
  }
}