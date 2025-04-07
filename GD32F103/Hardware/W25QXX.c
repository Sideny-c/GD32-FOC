#include "w25qxx.h"

void w25qxx_init(void){
	soft_spi_init();
}

// ���SR-1��BUSYλΪ1�Ļ���һֱ�ȴ���ֱ��BUSYλΪ0�������ȴ�
void w25qxx_wait_busy(void){
	while((w25qxx_read_sr(W25QXX_SR_ID_1) & 0x01) == 0x01){
		;
	}
}

// ��״̬�Ĵ���
uint8_t w25qxx_read_sr(uint8_t sregister_id){
	uint8_t command, result;
	switch(sregister_id){
		case W25QXX_SR_ID_1:
			command = W25QXX_READ_STATUS_REGISTER_1;
		break;
		case W25QXX_SR_ID_2:
			command = W25QXX_READ_STATUS_REGISTER_2;
		break;
		case W25QXX_SR_ID_3:
			command = W25QXX_READ_STATUS_REGISTER_3;
		break;
		default:
			command = W25QXX_READ_STATUS_REGISTER_1;
		break;
	}
	
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(command);
	result = w25qxx_swap(0xFF);
	w25qxx_cs_disable(W25QXX_ID_1);
	
	return result;
}

// ��flash������
// *p_buffer ���ص����ݵĴ��λ��
void w25qxx_read(uint8_t *p_buffer, uint32_t read_addr, uint16_t num_read_bytes){
	uint16_t i;
	
	w25qxx_cs_enable(W25QXX_ID_1);
	
	w25qxx_swap(W25QXX_READ_DATA); //���Ͷ����ݵ�ָ��
	w25qxx_swap(read_addr >> 16);  //����24bit��ַ
	w25qxx_swap(read_addr >> 8);
	w25qxx_swap(read_addr);
	
	for(i=0; i < num_read_bytes; i++){
		p_buffer[i] = w25qxx_swap(0xFF);
	}
	
	w25qxx_cs_disable(W25QXX_ID_1);
}

// 
uint8_t W25QXX_Buffer[4096];  //������Ŵ�sector������bytes
void w25qxx_write(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes){
	uint32_t sec_num;
	uint16_t sec_remain;
	uint16_t sec_off;
	uint16_t i;
	
	sec_num	= write_addr / 4096;              //Ҫд���λ�ô��ڵ�sec_num��������
	sec_off = write_addr % 4096;
	
	sec_remain = 4096 - sec_off;
	
	if(num_write_bytes <= sec_remain){
		w25qxx_read(W25QXX_Buffer, sec_num * 4096, 4096);  //���������ݶ�����
		
		for(i = 0; i < sec_remain; i++){
			if(W25QXX_Buffer[i + sec_off] != 0xFF)  //˵����������ĵ�i+sec_offλû�в���
				break;
		}
		
		if(i < sec_remain){ // ����û�в���
			w25qxx_erase_sector(sec_num * 4096);
			for(i = 0; i < sec_remain; i++){
				W25QXX_Buffer[i + sec_off] = p_buffer[i];
			}
			w25qxx_write_nocheck(W25QXX_Buffer, sec_num * 4096, 4096);
		}else{              // ����sec_remain�����ǲ�������
			w25qxx_write_nocheck(p_buffer, write_addr, num_write_bytes);
		}
	}else{
		w25qxx_read(W25QXX_Buffer, sec_num * 4096, 4096);  //���������ݶ�����
		
		for(i = 0; i < sec_remain; i++){
			if(W25QXX_Buffer[i + sec_off] != 0xFF)  //˵����������ĵ�i+sec_offλû�в���
				break;
		}
		
		if(i < sec_remain){ // ����û�в���
			w25qxx_erase_sector(sec_num * 4096);
			for(i = 0; i < sec_remain; i++){
				W25QXX_Buffer[i + sec_off] = p_buffer[i];
			}
			w25qxx_write_nocheck(W25QXX_Buffer, sec_num * 4096, 4096);
		}else{              // ����sec_remain�����ǲ�������
			w25qxx_write_nocheck(p_buffer, write_addr, sec_remain);
		}
		
		write_addr += sec_remain;
		p_buffer += sec_remain;
		num_write_bytes -= sec_remain;
		w25qxx_write(p_buffer, write_addr, num_write_bytes);
	}
		
	//�ж϶������������Ƿ�Ϊ0xFF
	;//�����Ƿ�ɾ��
	 //�ж��Ƿ��ҳ
}

// ����֮ǰ��ȷ������ɾ��
void w25qxx_write_nocheck(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes){
	uint16_t page_remain = 256 - write_addr % 256;
	
	if(num_write_bytes <= page_remain){
		w25qxx_write_page(p_buffer, write_addr, num_write_bytes);
	}else{
		w25qxx_write_page(p_buffer, write_addr, page_remain);
		p_buffer += page_remain;
		write_addr += page_remain;
		num_write_bytes -= page_remain;
		w25qxx_write_nocheck(p_buffer, write_addr, num_write_bytes);
	}
}

// page program
// ��֤û�п�ҳд��ǰ���µ��ô˺�����ĳ��ҳ��д����
void w25qxx_write_page(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes){
	uint16_t i;
	
	w25qxx_write_enable();
	
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_PAGE_PROGRAM);
	w25qxx_swap(write_addr >> 16);  //����24bit��ַ
	w25qxx_swap(write_addr >> 8);
	w25qxx_swap(write_addr);
	
	for(i = 0; i < num_write_bytes; i++){
		w25qxx_swap(p_buffer[i]);
	}
	w25qxx_cs_disable(W25QXX_ID_1);
	
	w25qxx_wait_busy();
}

void w25qxx_erase_sector(uint32_t sector_addr){
	w25qxx_write_enable();
	
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_SECTOR_ERASE_4KB);
	w25qxx_swap(sector_addr >> 16);
	w25qxx_swap(sector_addr >> 8);
	w25qxx_swap(sector_addr);
	w25qxx_cs_disable(W25QXX_ID_1);
	
	w25qxx_wait_busy();
}

void w25qxx_erase_chip(void){
	w25qxx_write_enable();
	
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_CHIP_ERASE);
	w25qxx_cs_disable(W25QXX_ID_1);
	
	w25qxx_wait_busy();
}

void w25qxx_write_enable(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_WRITE_ENABLE);
	w25qxx_cs_disable(W25QXX_ID_1);
}

void w25qxx_write_disable(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_WRITE_DISABLE);
	w25qxx_cs_disable(W25QXX_ID_1);
}

// �͵�������
void w25qxx_power_down(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_POWER_DOWN);
	w25qxx_cs_disable(W25QXX_ID_1);
}

// ����
void w25qxx_wake_up(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_RELEASE_POWER_DOWN_HPM_DEVICE_ID);
	w25qxx_cs_disable(W25QXX_ID_1);
}

/*
brief��ʹ��Ƭѡ����cs
cs_id: cs���ŵ���ţ����ڼ���w25qxx flash
*/
void w25qxx_cs_enable(uint8_t cs_id){
	switch(cs_id){
		case W25QXX_ID_1:
			soft_spi_begin();
		break;
		default:
			break;
	}
}

void w25qxx_cs_disable(uint8_t cs_id){
	switch(cs_id){
		case W25QXX_ID_1:
			soft_spi_end();
		break;
		default:
			break;
	}
}

/*

*/
uint8_t w25qxx_swap(uint8_t byte_to_send){
	return soft_spi_swap(byte_to_send);
}