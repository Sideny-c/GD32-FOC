#ifndef _SPI_H
#define _SPI_H

#include "gd32f10x.h"
#include "systick.h"

//定义表示具体IO口的资源宏
#define SPI_PORT GPIOA
#define SPI_MOSI GPIO_PIN_7
#define SPI_MISO GPIO_PIN_6
#define SPI_SCK  GPIO_PIN_5
#define SPI_CS   GPIO_PIN_4

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

void soft_spi_init(void);    // spi通信准备，初始化
void soft_spi_init_io(void); // 初始化spi通信用到的io口
void soft_spi_init_mode(uint8_t spi_mode); // 初始化spi通信模式，时钟相位和时钟极性

void soft_spi_begin(void);               // 开始spi通信
void soft_spi_w_cs(uint8_t bit_value);// 写片选cs口

uint8_t soft_spi_swap(uint8_t byte_to_send);
void soft_spi_w_sck(uint8_t bit_value);  // 写时钟口
void soft_spi_w_mosi(uint8_t bit_value);  // 写MOSI口
uint8_t soft_spi_r_miso(void);    // 读MISO口

void soft_spi_end(void);

#endif