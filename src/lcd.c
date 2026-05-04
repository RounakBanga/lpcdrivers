#include "lcd.h"
#include "gpio.h"

// Pin definitions
#define LCD_RS_PORT  0
#define LCD_RS_PIN   8
#define LCD_RW_PORT  0
#define LCD_RW_PIN   9
#define LCD_E_PORT   0
#define LCD_E_PIN    10

#define LCD_DATA_PORT 0
// Data pins: P0.0 to P0.7

// Timing delays (in microseconds - approximate)
static void delay_us(uint32_t us) {
    for(uint32_t i = 0; i < us * 4; i++);
}

static void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 4000; i++);
}

// Send enable pulse
static void LCD_EnablePulse(void) {
    GPIO_Write(LCD_E_PORT, LCD_E_PIN, HIGH);
    delay_us(1);
    GPIO_Write(LCD_E_PORT, LCD_E_PIN, LOW);
    delay_us(50);
}

// Write 8-bit data to LCD data pins (P0.0 - P0.7)
static void LCD_WriteData(uint8_t data) {
    for(int i = 0; i < 8; i++) {
        if(data & (1 << i)) {
            GPIO_Write(LCD_DATA_PORT, i, HIGH);
        } else {
            GPIO_Write(LCD_DATA_PORT, i, LOW);
        }
    }
}

// Send command to LCD
static void LCD_Command(uint8_t cmd) {
    GPIO_Write(LCD_RS_PORT, LCD_RS_PIN, LOW);   // RS = 0 for command
    GPIO_Write(LCD_RW_PORT, LCD_RW_PIN, LOW);   // RW = 0 for write
    
    LCD_WriteData(cmd);
    LCD_EnablePulse();
    
    if(cmd == LCD_CLEAR || cmd == LCD_HOME) {
        delay_ms(2);  // Clear/Home commands need more time
    } else {
        delay_us(50);
    }
}

// Send data (character) to LCD
static void LCD_Data(uint8_t data) {
    GPIO_Write(LCD_RS_PORT, LCD_RS_PIN, HIGH);  // RS = 1 for data
    GPIO_Write(LCD_RW_PORT, LCD_RW_PIN, LOW);   // RW = 0 for write
    
    LCD_WriteData(data);
    LCD_EnablePulse();
    delay_us(50);
}

// Initialize LCD in 8-bit mode
void LCD_Init(void) {
    // Configure all LCD pins as outputs
    for(int i = 0; i < 8; i++) {
        GPIO_SetDir(LCD_DATA_PORT, i, OUTPUT);  // D0-D7
    }
    GPIO_SetDir(LCD_RS_PORT, LCD_RS_PIN, OUTPUT);
    GPIO_SetDir(LCD_RW_PORT, LCD_RW_PIN, OUTPUT);
    GPIO_SetDir(LCD_E_PORT, LCD_E_PIN, OUTPUT);
    
    // Wait for LCD to power up
    delay_ms(50);
    
    // Initialization sequence for 8-bit mode
    LCD_Command(0x38);  // Function set: 8-bit, 2 lines, 5x8 font
    delay_ms(5);
    LCD_Command(0x38);
    delay_us(150);
    LCD_Command(0x38);
    
    LCD_Command(0x08);  // Display OFF
    LCD_Command(0x01);  // Clear display
    LCD_Command(0x06);  // Entry mode: increment cursor, no shift
    LCD_Command(0x0C);  // Display ON, cursor OFF, blink OFF
}

// Clear the display
void LCD_Clear(void) {
    LCD_Command(LCD_CLEAR);
}

// Set cursor position (row: 0-1, col: 0-15)
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;
    
    if(row == 0) {
        address = 0x80 + col;  // Row 0 starts at 0x00
    } else {
        address = 0xC0 + col;  // Row 1 starts at 0x40
    }
    
    LCD_Command(address);
}

// Print a single character
void LCD_PrintChar(char c) {
    LCD_Data(c);
}

// Print a string
void LCD_Print(const char *str) {
    while(*str) {
        LCD_PrintChar(*str++);
    }
}

// Print a number
void LCD_PrintNum(int num) {
    char buffer[16];
    int i = 0;
    int is_negative = 0;
    
    // Handle negative numbers
    if(num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // Handle zero
    if(num == 0) {
        buffer[i++] = '0';
    }
    
    // Convert number to string (reverse order)
    while(num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Add negative sign if needed
    if(is_negative) {
        buffer[i++] = '-';
    }
    
    // Print in correct order (reverse the buffer)
    for(int j = i - 1; j >= 0; j--) {
        LCD_PrintChar(buffer[j]);
    }
}