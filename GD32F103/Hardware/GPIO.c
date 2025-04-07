#include "GPIO.h"

void GPIO_Init_AF(void)
{
  rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOB|RCU_GPIOA);

	//gpio_pin_remap_config(GPIO_SPI0_REMAP, ENABLE);
	//gpio_pin_remap_config(GPIO_SWJ_NONJTRST_REMAP, ENABLE);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);

	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3);
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	gpio_bit_reset(GPIOB,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3);
	gpio_bit_set(GPIOA,GPIO_PIN_15);
}