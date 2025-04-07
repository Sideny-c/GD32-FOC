#include "main.h"


int Key_Num=0;
float angle =  0.0;
float all_angle =  0.0;
float speed =  0.0;
float speed_out = 0.0;
float local_out = 0.0;

float i_1=0,i_2=0,i_3=0;
uint16_t f_a=0,f_b=0;

int main(void)
{
	
		int PWM_a = 0 , PWM_b = 0 , PWM_c = 0 , angle_Now ;
    systick_config();
		//gpio_config();
		
	  delay_1ms(100);
		OLED_Init();				//OLED初始化
		OLED_ColorTurn(0);	//0正常显示，1 反色显示
		OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
		Key_Init();					//按键初始化
	  //usart_config();		//串口初始化 
		GPIO_Init_AF();			//GPIO重新映射
	  timer1_pwm_init(3-1, 2000-1, 0);//PWM初始化 MAX1000

		angle = angle_get();
		angle_init(angle);
		adc_config();
		//Timer2_init(10);
		//first_get(&f_a,&f_b);
    while(1)
		{		
			
			AD_Value[0] = read_adc0_data(ADC_CHANNEL_7);
			AD_Value[1] = read_adc0_data(ADC_CHANNEL_8);
			AD_Value[2] = read_adc0_data(ADC_CHANNEL_9);
			i_1 = AD_Value[0] - f_a;
			i_2 = AD_Value[1] - f_b;
			i_3 = 0-i_1-i_2;
			
			Key_Num = Key_Get();
			//`angle = angle_get();
			
			
			printf("Ua:%lf,Ub:%lf,Uc;%lf\n",Ua,Ub,Uc);
			if(angle>0){OLED_ShowString(0,0,"+");OLED_ShowFNum(10,0,angle,1,3,12);}
			else {OLED_ShowString(0,0,"-"); OLED_ShowFNum(10,0,fabs(angle),1,3,12);}
			if(all_angle>0){OLED_ShowString(0,1,"+");OLED_ShowFNum(10,1,all_angle,2,3,12);}
			else {OLED_ShowString(0,1,"-"); OLED_ShowFNum(10,1,fabs(all_angle),2,3,12);}
			
			
			
			OLED_ShowNum(97,0,AD_Value[0],4,12);
			OLED_ShowNum(97,1,AD_Value[1],4,12);
			OLED_ShowNum(97,2,AD_Value[2],4,12);
			
			if(Key_Num == 1)
			{
				Timer2_init(10);
				//PWM_a +=100;
			}
			if(Key_Num == 5)
			{
				//PWM_a -=100;
			}				
			//pwm_a_write(PWM_a);
			//pwm_b_write(PWM_b);
			//pwm_c_write(PWM_c);
			
			//OLED_ShowNum(10,2,PWM_a,4,12);
			
			//OLED_Pow(10,1);
			//OLED_Clear();
			//printf("Ud:%d\n",123);
			//OLED_ShowNum(97,0,12345,5,12);//12 x = 97 16 x = 88
			//OLED_ShowString(0,0,'1',8);
			//OLED_ShowChar(8,2,'A',12);
			//printf("Ud:%d\n",123);
			//angle = (int)angle_get();
			//OLED_ShowNum(97,0,angle,5,12);

			
		}
}



