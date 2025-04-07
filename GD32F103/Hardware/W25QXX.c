#include "w25qxx.h"

void w25qxx_init(void){
	soft_spi_init();
}

// 如果SR-1的BUSY位为1的话，一直等待，直到BUSY位为0，结束等待
void w25qxx_wait_busy(void){
	while((w25qxx_read_sr(W25QXX_SR_ID_1) & 0x01) == 0x01){
		;
	}
}

// 读状态寄存器
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

// 读flash的数据
// *p_buffer 读回的数据的存放位置
void w25qxx_read(uint8_t *p_buffer, uint32_t read_addr, uint16_t num_read_bytes){
	uint16_t i;
	
	w25qxx_cs_enable(W25QXX_ID_1);
	
	w25qxx_swap(W25QXX_READ_DATA); //发送读数据的指令
	w25qxx_swap(read_addr >> 16);  //发送24bit地址
	w25qxx_swap(read_addr >> 8);
	w25qxx_swap(read_addr);
	
	for(i=0; i < num_read_bytes; i++){
		p_buffer[i] = w25qxx_swap(0xFF);
	}
	
	w25qxx_cs_disable(W25QXX_ID_1);
}

// 
uint8_t W25QXX_Buffer[4096];  //用来存放从sector读出的bytes
void w25qxx_write(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes){
	uint32_t sec_num;
	uint16_t sec_remain;
	uint16_t sec_off;
	uint16_t i;
	
	sec_num	= write_addr / 4096;              //要写入的位置处在第sec_num个扇区上
	sec_off = write_addr % 4096;
	
	sec_remain = 4096 - sec_off;
	
	if(num_write_bytes <= sec_remain){
		w25qxx_read(W25QXX_Buffer, sec_num * 4096, 4096);  //扇区的数据读出来
		
		for(i = 0; i < sec_remain; i++){
			if(W25QXX_Buffer[i + sec_off] != 0xFF)  //说明这个扇区的第i+sec_off位没有擦除
				break;
		}
		
		if(i < sec_remain){ // 扇区没有擦除
			w25qxx_erase_sector(sec_num * 4096);
			for(i = 0; i < sec_remain; i++){
				W25QXX_Buffer[i + sec_off] = p_buffer[i];
			}
			w25qxx_write_nocheck(W25QXX_Buffer, sec_num * 4096, 4096);
		}else{              // 扇区sec_remain部分是擦除过的
			w25qxx_write_nocheck(p_buffer, write_addr, num_write_bytes);
		}
	}else{
		w25qxx_read(W25QXX_Buffer, sec_num * 4096, 4096);  //扇区的数据读出来
		
		for(i = 0; i < sec_remain; i++){
			if(W25QXX_Buffer[i + sec_off] != 0xFF)  //说明这个扇区的第i+sec_off位没有擦除
				break;
		}
		
		if(i < sec_remain){ // 扇区没有擦除
			w25qxx_erase_sector(sec_num * 4096);
			for(i = 0; i < sec_remain; i++){
				W25QXX_Buffer[i + sec_off] = p_buffer[i];
			}
			w25qxx_write_nocheck(W25QXX_Buffer, sec_num * 4096, 4096);
		}else{              // 扇区sec_remain部分是擦除过的
			w25qxx_write_nocheck(p_buffer, write_addr, sec_remain);
		}
		
		write_addr += sec_remain;
		p_buffer += sec_remain;
		num_write_bytes -= sec_remain;
		w25qxx_write(p_buffer, write_addr, num_write_bytes);
	}
		
	//判断读出来的数据是否都为0xFF
	;//扇区是否删除
	 //判断是否跨页
}

// 调用之前先确保扇区删除
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
// 保证没有跨页写的前提下调用此函数往某个页上写内容
void w25qxx_write_page(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes){
	uint16_t i;
	
	w25qxx_write_enable();
	
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_PAGE_PROGRAM);
	w25qxx_swap(write_addr >> 16);  //发送24bit地址
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

// 低电量休眠
void w25qxx_power_down(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_POWER_DOWN);
	w25qxx_cs_disable(W25QXX_ID_1);
}

// 唤醒
void w25qxx_wake_up(void){
	w25qxx_cs_enable(W25QXX_ID_1);
	w25qxx_swap(W25QXX_RELEASE_POWER_DOWN_HPM_DEVICE_ID);
	w25qxx_cs_disable(W25QXX_ID_1);
}

/*
brief：使能片选引脚cs
cs_id: cs引脚的序号，即第几个w25qxx flash
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