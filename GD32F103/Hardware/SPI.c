#include "spi.h"

uint8_t clock_polar;
uint8_t clock_phase;

// spiͨ��׼������ʼ��
void soft_spi_init(void){
	soft_spi_init_io();
	soft_spi_init_mode(SPI_MODE0);
}

// ��ʼ��spiͨ���õ���io��
void soft_spi_init_io(void){
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(SPI_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SPI_MOSI|SPI_SCK|SPI_CS);
	gpio_init(SPI_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, SPI_MISO);
}

// ��ʼ��spiͨ��ģʽ��ʱ����λ��ʱ�Ӽ���
void soft_spi_init_mode(uint8_t spi_mode){
	switch(spi_mode){
		case SPI_MODE0:
			clock_polar = 0;
			clock_phase = 0;
			break;
		case SPI_MODE1:
			clock_polar = 0;
			clock_phase = 1;
			break;
		case SPI_MODE2:
			clock_polar = 1;
			clock_phase = 0;
			break;
		case SPI_MODE3:
			clock_polar = 1;
			clock_phase = 1;
			break;
		default:
			break;
	}
}

// ��ʼspiͨ��
void soft_spi_begin(void){
	soft_spi_w_cs((bit_status)0); 
}

// дƬѡcs��
void soft_spi_w_cs(uint8_t bit_value){
	gpio_bit_write(SPI_PORT, SPI_CS, (bit_status)bit_value);
}

/*
��Ҫ������spi��������λ�Ĵ�����spi�ӻ���λ�Ĵ���������ֵ
*/
uint8_t soft_spi_swap(uint8_t byte_to_send){
	uint8_t byte_receive = 0x00;
	
	uint8_t i;
	for(i = 0; i < 8; i++){
		soft_spi_w_sck(clock_polar ? 1:0);
		delay_1ms(10);

		if(clock_phase){
			if(soft_spi_r_miso() == 1)byte_receive |= (0x80 >> i);
			soft_spi_w_sck(clock_phase ? 0 : 1);
			delay_1ms(10);
			soft_spi_w_mosi(byte_to_send & (0x80 >> i));
			
		}else{
			soft_spi_w_mosi(byte_to_send & (0x80 >> i));
			soft_spi_w_sck(clock_phase ? 0 : 1);
			delay_1ms(10);
			if(soft_spi_r_miso() == 1)byte_receive |= (0x80 >> i);
		}
		
	}
	
	return byte_receive;
}

// дʱ�ӿ�
void soft_spi_w_sck(uint8_t bit_value){
	gpio_bit_write(SPI_PORT, SPI_SCK, (bit_status)bit_value);
}

// дMOSI��
void soft_spi_w_mosi(uint8_t bit_value){
	gpio_bit_write(SPI_PORT, SPI_MOSI, (bit_status)bit_value);
}

// ��MISO��
uint8_t soft_spi_r_miso(void){
	return gpio_input_bit_get(SPI_PORT, SPI_MISO);    
}

void soft_spi_end(void){
	soft_spi_w_sck(clock_polar ? 1:0);
	soft_spi_w_cs((bit_status)1);
}