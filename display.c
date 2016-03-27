#include "display.h"

SDL_Surface *screen;
uint8_t __buffer[RESX][RESY];

int lcdInit()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
   
  if (!(screen = SDL_SetVideoMode(RESX, RESY, 32, SDL_HWSURFACE)))
    {
      SDL_Quit();
      return 1;
    }
  
  return 0;
}

void lcdClose()
{
  SDL_Quit();
}

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)			\
  (byte & 0x80 ? 1 : 0),			\
    (byte & 0x40 ? 1 : 0),			\
    (byte & 0x20 ? 1 : 0),			\
    (byte & 0x10 ? 1 : 0),			\
    (byte & 0x08 ? 1 : 0),			\
    (byte & 0x04 ? 1 : 0),			\
    (byte & 0x02 ? 1 : 0),			\
    (byte & 0x01 ? 1 : 0) 
void print_byte(uint8_t byte)
{
  printf(BYTETOBINARYPATTERN, BYTETOBINARY(byte));
}

uint8_t lcdGetPixel(uint x, uint y)
{
  return __buffer[x][y];
}

void lcdSetPixel(uint x, uint y, uint8_t color)
{
  __buffer[x][y] = color;
}

void lcdFill(uint8_t color)
{
  for (uint x = 0; x < RESX; x++) {
    for (uint y = 0; y < RESY; y++) {
      __buffer[x][y] = color;
    }
  }
}

float normalize(uint8_t num, uint8_t range)
{
  return (float)num / (float)range;
}

void set_pixel(uint x, uint y, Uint32 sdl_color)
{
    uint bpp = screen->format->BytesPerPixel;
    Uint32 *pix = (Uint32 *)((Uint8 *)screen->pixels + y * screen->pitch + x * bpp);
    *pix = sdl_color;
}

Uint32 to_sdl_color(uint8_t pixel)
{
  uint8_t r = (uint8_t)floor(normalize((pixel & 0b11100000) >> 5, (2 << 3)) * 255);
  uint8_t g = (uint8_t)floor(normalize((pixel & 0b00011100) >> 2, (2 << 3)) * 255);
  uint8_t b = (uint8_t)floor(normalize((pixel & 0b00000011), (2 << 2)) * 255);
  return SDL_MapRGB(screen->format, r, g, b);
}

void lcdDisplay()
{
  for (uint x = 0; x < RESX; x++) {
    for (uint y = 0; y < RESY; y++) {
      set_pixel(x, y, to_sdl_color(__buffer[x][y]));
    }
  }
  SDL_Flip(screen);
}
