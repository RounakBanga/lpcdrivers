#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD commands
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_CURSOR_OFF      0x0C
#define LCD_CURSOR_ON       0x0E
#define LCD_BLINK_ON        0x0F

// Function prototypes
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);
void LCD_PrintChar(char c);
void LCD_PrintNum(int num);

#endif