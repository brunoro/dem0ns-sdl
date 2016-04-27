#include <SDL.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define RESX 512
#define RESY 512

int lcdInit();
void lcdClose();
uint8_t lcdGetPixel(uint x, uint y);
void lcdSetPixel(uint x, uint y, uint8_t color);
void lcdFill(uint8_t color);
void lcdDisplay();

uint8_t *lcdGetBuffer();
