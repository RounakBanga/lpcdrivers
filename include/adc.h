#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// ADC channels (LPC1768 has 8 channels: AD0.0 to AD0.7)
#define ADC_CH0  0
#define ADC_CH1  1
#define ADC_CH2  2
#define ADC_CH3  3
#define ADC_CH4  4
#define ADC_CH5  5
#define ADC_CH6  6
#define ADC_CH7  7

// Function prototypes
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif