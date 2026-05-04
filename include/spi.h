#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// SPI modes
#define SPI_MODE0  0
#define SPI_MODE1  1
#define SPI_MODE2  2
#define SPI_MODE3  3

void SPI_Init(uint32_t frequency_hz, uint8_t mode);
uint8_t SPI_Transfer(uint8_t data);
void SPI_Write(uint8_t data);
uint8_t SPI_Read(void);

#endif