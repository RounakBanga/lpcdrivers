#ifndef OLED_H
#define OLED_H

#include <stdint.h>

#define OLED_WIDTH  128
#define OLED_HEIGHT 64

void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t color);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void OLED_DrawChar(uint8_t x, uint8_t y, char c, uint8_t color);
void OLED_DrawString(uint8_t x, uint8_t y, const char *str, uint8_t color);
void OLED_Update(void);

#endif