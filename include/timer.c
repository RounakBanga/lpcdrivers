#include "timer.h"
#include "LPC17xx.h"

// Get Timer peripheral pointer
static LPC_TIM_TypeDef* TIMER_GetPeripheral(uint8_t timer_num) {
    switch(timer_num) {
        case 0: return LPC_TIM0;
        case 1: return LPC_TIM1;
        case 2: return LPC_TIM2;
        case 3: return LPC_TIM3;
        default: return LPC_TIM0;
    }
}

// Initialize timer with interval in microseconds
void TIMER_Init(uint8_t timer_num, uint32_t interval_us) {
    LPC_TIM_TypeDef *timer = TIMER_GetPeripheral(timer_num);
    
    // Power on timer
    if(timer_num == 0) LPC_SC->PCONP |= (1 << 1);
    else if(timer_num == 1) LPC_SC->PCONP |= (1 << 2);
    else if(timer_num == 2) LPC_SC->PCONP |= (1 << 22);
    else if(timer_num == 3) LPC_SC->PCONP |= (1 << 23);
    
    // Set prescaler (assuming PCLK = 96MHz, we want 1MHz timer clock)
    timer->PR = 96 - 1;  // 96MHz / 96 = 1MHz = 1us per tick
    
    // Set match value
    timer->MR0 = interval_us;
    
    // Reset on match
    timer->MCR = 0x02;
    
    // Reset timer
    timer->TCR = 0x02;
    timer->TCR = 0x00;
}

// Start timer
void TIMER_Start(uint8_t timer_num) {
    LPC_TIM_TypeDef *timer = TIMER_GetPeripheral(timer_num);
    timer->TCR = 0x01;  // Enable timer
}

// Stop timer
void TIMER_Stop(uint8_t timer_num) {
    LPC_TIM_TypeDef *timer = TIMER_GetPeripheral(timer_num);
    timer->TCR = 0x00;  // Disable timer
}

// Reset timer
void TIMER_Reset(uint8_t timer_num) {
    LPC_TIM_TypeDef *timer = TIMER_GetPeripheral(timer_num);
    timer->TCR = 0x02;  // Reset
    timer->TCR = 0x00;
}

// Get current timer value
uint32_t TIMER_GetValue(uint8_t timer_num) {
    LPC_TIM_TypeDef *timer = TIMER_GetPeripheral(timer_num);
    return timer->TC;
}

// Blocking delay in milliseconds
void TIMER_DelayMs(uint32_t ms) {
    TIMER_Init(0, 1000);  // 1ms ticks
    TIMER_Reset(0);
    TIMER_Start(0);
    
    for(uint32_t i = 0; i < ms; i++) {
        while(TIMER_GetValue(0) < 1000);
        TIMER_Reset(0);
    }
    
    TIMER_Stop(0);
}

// Blocking delay in microseconds
void TIMER_DelayUs(uint32_t us) {
    TIMER_Init(0, us);
    TIMER_Reset(0);
    TIMER_Start(0);
    
    while(TIMER_GetValue(0) < us);
    
    TIMER_Stop(0);
}

// PWM functions using Timer1/PWM1
void PWM_Init(uint32_t frequency_hz) {
    // Power on PWM1
    LPC_SC->PCONP |= (1 << 6);
    
    // Set PCLK for PWM1
    LPC_SC->PCLKSEL0 &= ~(3 << 12);
    LPC_SC->PCLKSEL0 |= (1 << 12);  // PCLK = CCLK
    
    // Configure PWM pins (P2.0 - P2.5 for PWM1.1 - PWM1.6)
    LPC_PINCON->PINSEL4 &= ~0x00000FFF;
    LPC_PINCON->PINSEL4 |= 0x00000555;  // PWM1.1-1.6
    
    // Set PWM period (96MHz / frequency_hz)
    LPC_PWM1->MR0 = 96000000 / frequency_hz;
    
    // Reset on MR0
    LPC_PWM1->MCR = 0x02;
    
    // Enable PWM output
    LPC_PWM1->PCR = 0x00;
    
    // Reset and enable counter
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;  // Enable PWM and counter
}

// Set duty cycle (0-100%)
void PWM_SetDutyCycle(uint8_t channel, uint8_t duty_percent) {
    if(duty_percent > 100) duty_percent = 100;
    
    uint32_t pulse_width = (LPC_PWM1->MR0 * duty_percent) / 100;
    
    switch(channel) {
        case 1: LPC_PWM1->MR1 = pulse_width; break;
        case 2: LPC_PWM1->MR2 = pulse_width; break;
        case 3: LPC_PWM1->MR3 = pulse_width; break;
        case 4: LPC_PWM1->MR4 = pulse_width; break;
        case 5: LPC_PWM1->MR5 = pulse_width; break;
        case 6: LPC_PWM1->MR6 = pulse_width; break;
    }
    
    // Latch new value
    LPC_PWM1->LER |= (1 << channel);
}

// Start PWM
void PWM_Start(void) {
    LPC_PWM1->PCR = 0x7E;  // Enable all PWM channels
}

// Stop PWM
void PWM_Stop(void) {
    LPC_PWM1->PCR = 0x00;  // Disable all PWM channels
}