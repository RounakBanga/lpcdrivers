#include "oled.h"
#include "spi.h"
#include "gpio.h"

// OLED pins
#define OLED_DC_PORT   0
#define OLED_DC_PIN    6
#define OLED_RST_PORT  0
#define OLED_RST_PIN   7
#define OLED_CS_PORT   0
#define OLED_CS_PIN    16

// Display buffer
static uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

// Send command
static void OLED_Command(uint8_t cmd) {
    GPIO_Write(OLED_DC_PORT, OLED_DC_PIN, LOW);   // Command mode
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, LOW);   // Select
    SPI_Write(cmd);
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, HIGH);  // Deselect
}

// Send data
static void OLED_Data(uint8_t data) {
    GPIO_Write(OLED_DC_PORT, OLED_DC_PIN, HIGH);  // Data mode
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, LOW);   // Select
    SPI_Write(data);
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, HIGH);  // Deselect
}

// Initialize OLED
void OLED_Init(void) {
    // Configure control pins
    GPIO_SetDir(OLED_DC_PORT, OLED_DC_PIN, OUTPUT);
    GPIO_SetDir(OLED_RST_PORT, OLED_RST_PIN, OUTPUT);
    GPIO_SetDir(OLED_CS_PORT, OLED_CS_PIN, OUTPUT);
    
    // Initialize SPI
    SPI_Init(8000000, SPI_MODE0);  // 8MHz
    
    // Reset display
    GPIO_Write(OLED_RST_PORT, OLED_RST_PIN, HIGH);
    for(volatile int i = 0; i < 100000; i++);
    GPIO_Write(OLED_RST_PORT, OLED_RST_PIN, LOW);
    for(volatile int i = 0; i < 100000; i++);
    GPIO_Write(OLED_RST_PORT, OLED_RST_PIN, HIGH);
    
    // Initialization sequence for SSD1306
    OLED_Command(0xAE);  // Display off
    OLED_Command(0xD5);  // Set display clock
    OLED_Command(0x80);
    OLED_Command(0xA8);  // Set multiplex ratio
    OLED_Command(0x3F);
    OLED_Command(0xD3);  // Set display offset
    OLED_Command(0x00);
    OLED_Command(0x40);  // Set start line
    OLED_Command(0x8D);  // Charge pump
    OLED_Command(0x14);
    OLED_Command(0x20);  // Memory mode
    OLED_Command(0x00);
    OLED_Command(0xA1);  // Segment remap
    OLED_Command(0xC8);  // COM scan direction
    OLED_Command(0xDA);  // COM pins config
    OLED_Command(0x12);
    OLED_Command(0x81);  // Contrast
    OLED_Command(0xCF);
    OLED_Command(0xD9);  // Pre-charge
    OLED_Command(0xF1);
    OLED_Command(0xDB);  // VCOMH deselect
    OLED_Command(0x40);
    OLED_Command(0xA4);  // Display all on resume
    OLED_Command(0xA6);  // Normal display
    OLED_Command(0xAF);  // Display on
    
    OLED_Clear();
}

// Clear display buffer
void OLED_Clear(void) {
    for(int i = 0;  i< sizeof(oled_buffer); i++) {
        oled_buffer[i] = 0x00;
    }
}

// Set pixel in buffer
void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t color) {
    if(x >= OLED_WIDTH || y >= OLED_HEIGHT) return;
    
    if(color) {
        oled_buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y % 8));
    } else {
        oled_buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}

// Draw line (Bresenham's algorithm)
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dx_abs = (dx < 0) ? -dx : dx;
    int dy_abs = (dy < 0) ? -dy : dy;
    
    int x = x1;
    int y = y1;
    
    OLED_SetPixel(x, y, color);
    
    if(dx_abs >= dy_abs) {
        int p = 2 * dy_abs - dx_abs;
        for(int i = 0; i < dx_abs; i++) {
            x += (dx < 0) ? -1 : 1;
            if(p >= 0) {
                y += (dy < 0) ? -1 : 1;
                p -= 2 * dx_abs;
            }
            p += 2 * dy_abs;
            OLED_SetPixel(x, y, color);
        }
    } else {
        int p = 2 * dx_abs - dy_abs;
        for(int i = 0; i < dy_abs; i++) {
            y += (dy < 0) ? -1 : 1;
            if(p >= 0) {
                x += (dx < 0) ? -1 : 1;
                p -= 2 * dy_abs;
            }
            p += 2 * dx_abs;
            OLED_SetPixel(x, y, color);
        }
    }
}

// Draw rectangle
void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    OLED_DrawLine(x, y, x + w, y, color);
    OLED_DrawLine(x + w, y, x + w, y + h, color);
    OLED_DrawLine(x + w, y + h, x, y + h, color);
    OLED_DrawLine(x, y + h, x, y, color);
}

// Fill rectangle
void OLED_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
    for(uint8_t i = x; i < x + w; i++) {
        for(uint8_t j = y; j < y + h; j++) {
            OLED_SetPixel(i, j, color);
        }
    }
}

// Simple 5x7 font (just for demo - add full font later)
void OLED_DrawChar(uint8_t x, uint8_t y, char c, uint8_t color) {
    // Placeholder - would need font data
    OLED_FillRect(x, y, 5, 7, color);
}

// Draw string
void OLED_DrawString(uint8_t x, uint8_t y, const char *str, uint8_t color) {
    while(*str) {
        OLED_DrawChar(x, y, *str++, color);
        x += 6;
    }
}

// Update display from buffer
void OLED_Update(void) {
    OLED_Command(0x21);  // Column address
    OLED_Command(0);
    OLED_Command(127);
    
    OLED_Command(0x22);  // Page address
    OLED_Command(0);
    OLED_Command(7);
    
    GPIO_Write(OLED_DC_PORT, OLED_DC_PIN, HIGH);  // Data mode
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, LOW);
    
    for(int i = 0; i < sizeof(oled_buffer); i++) {
        SPI_Write(oled_buffer[i]);
    }
    
    GPIO_Write(OLED_CS_PORT, OLED_CS_PIN, HIGH);
}