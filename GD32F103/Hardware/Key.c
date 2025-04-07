#include "key.h"


void Key_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOB);
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}


uint8_t Key_Get(void)
{
	uint8_t Key_Num=0;
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_12)==0)
	{
		delay_1ms(10);
		while(gpio_input_bit_get(GPIOB,GPIO_PIN_12)==0);
		return Key_Num=1;//����
	}
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==0)
	{
		delay_1ms(10);
		while(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==0);
		return Key_Num=2;//��
	}
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_14)==0)
	{
		delay_1ms(10);
		while(gpio_input_bit_get(GPIOB,GPIO_PIN_14)==0);
		return Key_Num=3;//����
	}
	else if(gpio_input_bit_get(GPIOB,GPIO_PIN_15)==0)
	{
		delay_1ms(10);
		while(gpio_input_bit_get(GPIOB,GPIO_PIN_15)==0);
		return Key_Num=4;//����
	}
	else if(gpio_input_bit_get(GPIOA,GPIO_PIN_8)==0)
	{
		delay_1ms(10);
		while(gpio_input_bit_get(GPIOA,GPIO_PIN_8)==0);
		return Key_Num=5;//����
	}
	return 0;
}


char Key_Scan(void)
{
    static char key_up=1;

    if(key_up&&(KB1||KB2||KB3||KB4||KB5))
    {
        delay_1ms(10);//ȥ����
        key_up=0;
        if     (KB1)
            return KEY_UP;
        else if(KB2)
            return KEY_DOWN;
        else if(KB3)
            return KEY_LEFT;
        else if(KB4)
            return KEY_RIGHT;
        else if(KB5)
            return KEY_MID;
    }
    else if(!KB1&&!KB2&&!KB3&&!KB4&&!KB5)
    {
        key_up=1;
    }
    return 0;//�ް�������
}


unsigned char KEY_Read(KEYn_e KEYno)
{
    switch(KEYno)
    {
      case KEY0:
        return gpio_input_bit_get(GPIOB,KEY0p);//ĸ���ϰ���0 right
      break;

      case KEY1:
        return gpio_input_bit_get(GPIOB,KEY1p);//ĸ���ϰ���1  mid
      break;

      case KEY2:
        return gpio_input_bit_get(GPIOB,KEY2p);//ĸ���ϰ���2  up
      break;

      case KEY3:
        return gpio_input_bit_get(GPIOB,KEY3p);//start
      break;

      case KEY4:
        return gpio_input_bit_get(GPIOA,KEY4p);// left
      break;

      default:
        return 0XFF;
    }
    return 0;
}

