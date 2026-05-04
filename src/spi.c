#include "spi.h"
#include "LPC17xx.h"

void SPI_Init(uint32_t frequency_hz, uint8_t mode) {
    // Power on SSP0 (SPI)
    LPC_SC->PCONP |= (1 << 21);
    
    // Set PCLK for SSP0
    LPC_SC->PCLKSEL1 &= ~(3 << 10);
    LPC_SC->PCLKSEL1 |= (1 << 10);  // PCLK = CCLK
    
    // Configure SPI pins (P0.15=SCK, P0.17=MISO, P0.18=MOSI)
    LPC_PINCON->PINSEL0 &= ~((3 << 30));
    LPC_PINCON->PINSEL0 |= (2 << 30);   // SCK
    LPC_PINCON->PINSEL1 &= ~((3 << 2) | (3 << 4));
    LPC_PINCON->PINSEL1 |= (2 << 2) | (2 << 4);  // MISO, MOSI
    
    // Calculate prescaler
    uint8_t prescaler = 96000000 / frequency_hz;
    if(prescaler < 2) prescaler = 2;
    if(prescaler > 254) prescaler = 254;
    
    // Configure SSP0
    LPC_SSP0->CR0 = 0x07;  // 8-bit, SPI mode
    LPC_SSP0->CPSR = prescaler;
    LPC_SSP0->CR1 = 0x02;  // Enable SSP, master mode
}

uint8_t SPI_Transfer(uint8_t data) {
    // Wait until transmit FIFO not full
    while(!(LPC_SSP0->SR & (1 << 1)));
    
    LPC_SSP0->DR = data;
    
    // Wait until receive FIFO not empty
    while(!(LPC_SSP0->SR & (1 << 2)));
    
    return LPC_SSP0->DR;
}

void SPI_Write(uint8_t data) {
    SPI_Transfer(data);
}

uint8_t SPI_Read(void) {
    return SPI_Transfer(0xFF);
}