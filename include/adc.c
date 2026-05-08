#include "adc.h"
#include "LPC17xx.h"

/*
 * ADC Driver for LPC17xx
 *
 * ADC_Init:
 * Powers on the ADC peripheral via bit 12 of the PCONP register, then configures
 * the ADC clock by clearing bits 25:24 of PCLKSEL0 and setting them to 01, which
 * selects PCLK/8. The ADCR register is then written to power up the ADC (bit 21)
 * and set CLKDIV to 4 (bits 15:8), resulting in a final ADC clock of
 * PCLK / 8 / 5 = ~1MHz.
 *
 * ADC_Read:
 * Clears the lower 8 channel-select bits of ADCR, sets the bit corresponding to
 * the requested channel, then sets bit 24 to trigger a conversion. Polls bit 31
 * of ADGDR until the hardware signals completion, then reads ADGDR, shifts the
 * result right by 4, and masks to 12 bits to extract the final value.
 */

void ADC_Init(void) {
    LPC_SC->PCONP |= (1 << 12);
    LPC_SC->PCLKSEL0 &= ~(3 << 24);
    LPC_SC->PCLKSEL0 |= (1 << 24);
    LPC_ADC->ADCR = (1 << 21) | (4 << 8);
}

uint16_t ADC_Read(uint8_t channel) {
    uint32_t result;
    LPC_ADC->ADCR &= ~0xFF;
    LPC_ADC->ADCR |= (1 << channel);
    LPC_ADC->ADCR |= (1 << 24);
    while(!(LPC_ADC->ADGDR & (1 << 31)));
    result = LPC_ADC->ADGDR;
    result = (result >> 4) & 0xFFF;
    return (uint16_t)result;
}