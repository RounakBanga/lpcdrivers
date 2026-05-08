#include "gpio.h"
#include "adc.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"

int main() {
    uint16_t sensor_value;
    
    // Initialize all peripherals
    GPIO_SetDir(1, 18, OUTPUT);
    ADC_Init();
    LCD_Init();
    UART_Init(UART0, BAUD_9600);
    
    // Welcome messages
    LCD_Clear();
    LCD_Print("System Ready!");
    UART_SendString(UART0, "LPC1768 Driver Library Initialized\r\n");
    
    TIMER_DelayMs(2000);
    LCD_Clear();
    
    while(1) {
        // Read sensor
        sensor_value = ADC_Read(ADC_CH0);
        
        // Display on LCD
        LCD_SetCursor(0, 0);
        LCD_Print("ADC: ");
        LCD_PrintNum(sensor_value);
        LCD_Print("    ");
        
        // Send via UART
        UART_Printf(UART0, "Sensor: %d\r\n", sensor_value);
        
        // LED control
        if(sensor_value > 2048) {
            GPIO_Write(1, 18, HIGH);
        } else {
            GPIO_Write(1, 18, LOW);
        }
        
        TIMER_DelayMs(1000);  // Update every second
    }
    
    return 0;
}