#include "gpio.h"
#include "LPC17xx.h"

void GPIO_SetDir(uint8_t port, uint8_t pin, uint8_t dir) {
    volatile uint32_t *fiodir;
    
    // Select the correct port
    switch(port) {
        case 0: fiodir = &LPC_GPIO0->FIODIR; break;
        case 1: fiodir = &LPC_GPIO1->FIODIR; break;
        case 2: fiodir = &LPC_GPIO2->FIODIR; break;
        case 3: fiodir = &LPC_GPIO3->FIODIR; break;
        case 4: fiodir = &LPC_GPIO4->FIODIR; break;
        default: return;
    }
    


    // Set direction
    if (dir == OUTPUT) {
        *fiodir |= (1 << pin);
    } else {
        *fiodir &= ~(1 << pin);
    }
}

void GPIO_Write(uint8_t port, uint8_t pin, uint8_t value) {
    volatile uint32_t *fioset, *fioclr;
    
    // Select the correct port
    switch(port) {
        case 0: 
            fioset = &LPC_GPIO0->FIOSET;
            fioclr = &LPC_GPIO0->FIOCLR;
            break;
        case 1: 
            fioset = &LPC_GPIO1->FIOSET;
            fioclr = &LPC_GPIO1->FIOCLR;
            break;
        case 2: 
            fioset = &LPC_GPIO2->FIOSET;
            fioclr = &LPC_GPIO2->FIOCLR;
            break;
        case 3: 
            fioset = &LPC_GPIO3->FIOSET;
            fioclr = &LPC_GPIO3->FIOCLR;
            break;
        case 4: 
            fioset = &LPC_GPIO4->FIOSET;
            fioclr = &LPC_GPIO4->FIOCLR;
            break;
        default: return;
    }
    
    // Write value
    if (value == HIGH) {
        *fioset = (1 << pin);
    } else {
        *fioclr = (1 << pin);
    }
}

uint8_t GPIO_Read(uint8_t port, uint8_t pin) {
    volatile uint32_t *fiopin;
    
    // Select the correct port
    switch(port) {
        case 0: fiopin = &LPC_GPIO0->FIOPIN; break;
        case 1: fiopin = &LPC_GPIO1->FIOPIN; break;
        case 2: fiopin = &LPC_GPIO2->FIOPIN; break;
        case 3: fiopin = &LPC_GPIO3->FIOPIN; break;
        case 4: fiopin = &LPC_GPIO4->FIOPIN; break;
        default: return 0;
    }
    
    // Read pin value
    return (*fiopin & (1 << pin)) ? HIGH : LOW;
}