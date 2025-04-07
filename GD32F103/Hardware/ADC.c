#include "adc.h"

uint16_t AD_Value[3];
/* 初始化IO口为模拟输入模式 */
void gpio_config()
{
		rcu_periph_clock_enable(RCU_GPIOB|RCU_GPIOA);  //1.使能rcu时钟
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_7);//789
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0 |GPIO_PIN_1);  //2.初始化gpio模式
}

/* 初始化adc0 */
void adc_config()
{
	gpio_config();
	rcu_periph_clock_enable(RCU_ADC0);            // 使能时钟
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);  // 配置ADC的时钟
	
	/* 配置ADC0的参数 */
	adc_mode_config(ADC_MODE_FREE);                                  // 自由模式
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);        
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); //关闭连续模式
	
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);      //对齐方式
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); // 触发方式,软件触发
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);  // 使能外部触发
	
	/* 使能ADC0 */
	adc_enable(ADC0);
	delay_1ms(2);     //等待稳定
	
	/* 自校准 */
	adc_calibration_enable(ADC0);
}

// 读取ADC0转换的结果  //789
uint16_t read_adc0_data(uint8_t adc_channel){
	/*配置adc0的通道*/
	adc_regular_channel_config(ADC0, 0, adc_channel, ADC_SAMPLETIME_1POINT5);
	
	/*触发adc0的转换*/
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	
	/*等待EOC置位，即ADC0转换完成*/
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	/*清零EOC*/
	adc_flag_clear(ADC0, ADC_FLAG_EOC);
	
	/*读常规通道数据寄存器值并返回*/
	return adc_regular_data_read(ADC0);
}

/*获取光敏电阻的阻值, 
* return:光敏电阻阻值，单位是千欧
*/
float get_photo_r(void)
{
	uint16_t adc_value = 0;
	uint8_t i;
	for(i=0; i<8; i++)
	{
    	adc_value += read_adc0_data(ADC_CHANNEL_5);
	}
	
	adc_value = adc_value / 8;
	
	float v_photo_res = adc_value * 3.3f / 4096.0f;  //得到模数转换的结果对应的电压值
	float result = (v_photo_res * 10) / (3 - v_photo_res);
	return result; 
}

/*获取可调电阻的阻值, 
* return:可调电阻阻值，单位是千欧
*/
float get_var_r(void)
{      
	uint16_t adc_value = 0;
	uint8_t i;
	for(i=0; i<8; i++)
	{
    	adc_value += read_adc0_data(ADC_CHANNEL_4);
	}
	
	adc_value = adc_value / 8;
	
	float v_var_res = adc_value * 3.3f / 4096.0f;  //得到模数转换的结果对应的电压值
	float result = (v_var_res * 10) / (3 - v_var_res);
	return result; 
}

//上电读取偏置电压
void first_get(uint16_t *First_a,uint16_t *First_b)
{
	for(int i=0;i<16;i++)
	{
		*First_a += AD_Value[0];
		*First_b += AD_Value[1];
	}
	*First_a = *First_a>>4;
	*First_b = *First_b>>4;
}