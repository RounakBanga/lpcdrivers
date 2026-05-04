#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Direction modes
#define INPUT  0
#define OUTPUT 1

// Pin states
#define LOW  0
#define HIGH 1

// Function prototypes
void GPIO_SetDir(uint8_t port, uint8_t pin, uint8_t dir);
void GPIO_Write(uint8_t port, uint8_t pin, uint8_t value);
uint8_t GPIO_Read(uint8_t port, uint8_t pin);

#endif