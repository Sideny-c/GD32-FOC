#ifndef _W25QXX_H
#define _W25QXX_H

#include "gd32f10x.h"
#include "w25qxx_ins.h"
#include "spi.h"

#define W25QXX_ID_1           1
#define W25QXX_SR_ID_1        1
#define W25QXX_SR_ID_2        2
#define W25QXX_SR_ID_3        3

void w25qxx_init(void);

void w25qxx_wait_busy(void);
uint8_t w25qxx_read_sr(uint8_t sregister_id);  // ¶Á×´Ì¬¼Ä´æÆ÷
	
void w25qxx_read(uint8_t *p_buffer, uint32_t read_addr, uint16_t num_read_bytes);

void w25qxx_write(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes);
void w25qxx_write_nocheck(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes); //
void w25qxx_write_page(uint8_t *p_buffer, uint32_t write_addr, uint16_t num_write_bytes);   // page program

void w25qxx_erase_sector(uint32_t sector_addr);
void w25qxx_erase_chip(void);

void w25qxx_write_enable(void);
void w25qxx_write_disable(void);

void w25qxx_power_down(void);
void w25qxx_wake_up(void);

void w25qxx_cs_enable(uint8_t cs_id);
void w25qxx_cs_disable(uint8_t cs_id);
uint8_t w25qxx_swap(uint8_t byte_to_send);


#endif
