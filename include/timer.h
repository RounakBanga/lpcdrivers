#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Timer channels
#define TIMER0  0
#define TIMER1  1
#define TIMER2  2
#define TIMER3  3

// PWM channels (Timer1 has PWM)
#define PWM_CH1  1
#define PWM_CH2  2
#define PWM_CH3  3
#define PWM_CH4  4
#define PWM_CH5  5
#define PWM_CH6  6

// Function prototypes
void TIMER_Init(uint8_t timer_num, uint32_t interval_us);
void TIMER_Start(uint8_t timer_num);
void TIMER_Stop(uint8_t timer_num);
void TIMER_Reset(uint8_t timer_num);
uint32_t TIMER_GetValue(uint8_t timer_num);
void TIMER_DelayMs(uint32_t ms);
void TIMER_DelayUs(uint32_t us);

// PWM functions
void PWM_Init(uint32_t frequency_hz);
void PWM_SetDutyCycle(uint8_t channel, uint8_t duty_percent);
void PWM_Start(void);
void PWM_Stop(void);

#endif