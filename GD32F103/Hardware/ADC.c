#include "adc.h"

uint16_t AD_Value[3];
/* ��ʼ��IO��Ϊģ������ģʽ */
void gpio_config()
{
		rcu_periph_clock_enable(RCU_GPIOB|RCU_GPIOA);  //1.ʹ��rcuʱ��
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_7);//789
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0 |GPIO_PIN_1);  //2.��ʼ��gpioģʽ
}

/* ��ʼ��adc0 */
void adc_config()
{
	gpio_config();
	rcu_periph_clock_enable(RCU_ADC0);            // ʹ��ʱ��
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);  // ����ADC��ʱ��
	
	/* ����ADC0�Ĳ��� */
	adc_mode_config(ADC_MODE_FREE);                                  // ����ģʽ
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);        
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); //�ر�����ģʽ
	
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);      //���뷽ʽ
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); // ������ʽ,�������
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);  // ʹ���ⲿ����
	
	/* ʹ��ADC0 */
	adc_enable(ADC0);
	delay_1ms(2);     //�ȴ��ȶ�
	
	/* ��У׼ */
	adc_calibration_enable(ADC0);
}

// ��ȡADC0ת���Ľ��  //789
uint16_t read_adc0_data(uint8_t adc_channel){
	/*����adc0��ͨ��*/
	adc_regular_channel_config(ADC0, 0, adc_channel, ADC_SAMPLETIME_1POINT5);
	
	/*����adc0��ת��*/
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	
	/*�ȴ�EOC��λ����ADC0ת�����*/
	while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
	/*����EOC*/
	adc_flag_clear(ADC0, ADC_FLAG_EOC);
	
	/*������ͨ�����ݼĴ���ֵ������*/
	return adc_regular_data_read(ADC0);
}

/*��ȡ�����������ֵ, 
* return:����������ֵ����λ��ǧŷ
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
	
	float v_photo_res = adc_value * 3.3f / 4096.0f;  //�õ�ģ��ת���Ľ����Ӧ�ĵ�ѹֵ
	float result = (v_photo_res * 10) / (3 - v_photo_res);
	return result; 
}

/*��ȡ�ɵ��������ֵ, 
* return:�ɵ�������ֵ����λ��ǧŷ
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
	
	float v_var_res = adc_value * 3.3f / 4096.0f;  //�õ�ģ��ת���Ľ����Ӧ�ĵ�ѹֵ
	float result = (v_var_res * 10) / (3 - v_var_res);
	return result; 
}

//�ϵ��ȡƫ�õ�ѹ
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