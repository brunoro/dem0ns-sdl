#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h> 
#include <stdio.h>
#include <time.h>

#include "display.h"
#include "input.h"

#define MIN(A,B) (A < B ? A : B)
#define MAX(A,B) (A >= B ? A : B)
#define BETWEEN(X,START,END) (MIN(MAX(X, START), END))

#define MIN_NUM_STATES 4
#define MAX_NUM_STATES 24

#define NEXT_STATE(STATE) ((STATE + color_step) % max_color)
#define RAND_COLOR ((rand() % num_states) * color_step)

#define MIN_BLOCK_SIZE 2
#define MAX_BLOCK_SIZE 8
#define MAX_SIZE_X (RESX / MIN_BLOCK_SIZE)
#define MAX_SIZE_Y (RESY / MIN_BLOCK_SIZE)

#define GET_STATE(X,Y) (lcdGetPixel(X * block_size, Y * block_size))
#define GET_NEXT_STATE(X,Y) (new_buffer[X][Y])
#define SET_NEXT_STATE(X,Y,VAL) (new_buffer[X][Y] = VAL)

// states <-> colors
uint8_t num_states;
uint8_t max_color;
uint8_t color_step;

// pixels <-> blocks
uint block_size;
uint size_x;
uint size_y;
uint pad_x;
uint pad_y;

uint8_t new_buffer[MAX_SIZE_X][MAX_SIZE_Y];
bool stop;
    
static void handle_input();
static void reset();
static void step();
static void draw_block(uint i, uint j, uint8_t color);
static void draw();

Uint32 previousFrameTime;

// TODO: wrap GetTicks on a helper
Uint32 getDeltaTime()
{
  Uint32 deltaTime = SDL_GetTicks() - previousFrameTime;
  return deltaTime;
}

void ram(void)
{
  Uint32 fps = 60;
  previousFrameTime = SDL_GetTicks();

  num_states = MIN_NUM_STATES;
  block_size = MIN_BLOCK_SIZE;
  stop = false;

  reset();
  draw();
  handle_input();
  step();
  while (!stop)
    {
      Uint32 delta = getDeltaTime();
      Uint32 min_delta = (1000 / fps);
      if (delta > min_delta) // 
	{
	  draw();
	  handle_input();
	  step();

	  previousFrameTime = SDL_GetTicks();
	}
    }
}

void handle_input()
{
  switch (getInputRaw()) {
  case BTN_UP:
    block_size = BETWEEN(block_size - 1, MIN_BLOCK_SIZE, MAX_BLOCK_SIZE);
    reset();
    break;
  case BTN_DOWN:
    block_size = BETWEEN(block_size + 1, MIN_BLOCK_SIZE, MAX_BLOCK_SIZE);
    reset();
    break;
  case BTN_LEFT:
    num_states = BETWEEN(num_states - 1, MIN_NUM_STATES, MAX_NUM_STATES);
    reset();
    break;
  case BTN_RIGHT:
    num_states = BETWEEN(num_states + 1, MIN_NUM_STATES, MAX_NUM_STATES);
    reset();
    break;
  case BTN_ENTER:
    stop = true;
    break;
  }
}

void reset()
{
  max_color = ((((UINT8_MAX / 2) * 2) / num_states) * num_states);
  color_step = (max_color / num_states);

  size_x = RESX / block_size;
  size_y = RESY / block_size;
  printf("block_size: %u\n", block_size);
  printf("%u, %u -> %u, %u\n", RESX, RESY, size_x, size_y);

  pad_x = (RESX - size_x * block_size) / 2;
  pad_y = (RESY - size_y * block_size) / 2;

  for (uint i = 0; i < size_x; i++) {
    for (uint j = 0; j < size_y; j++) {

      uint8_t alo = rand();
      //uint8_t rando = (alo % num_states) * color_step;
      uint8_t rando = (alo % num_states) * color_step;
      SET_NEXT_STATE(i, j, rando);
    }
  }
    
  lcdFill(0);
}

void step()
{
  for (uint i = 0; i < size_x; i++) {
    for (uint j = 0; j < size_y; j++) {
      uint min_x = MAX(i - 1, 0);
      uint max_x = MIN(i + 1, size_x - 1);

      uint min_y = MAX(j - 1, 0);
      uint max_y = MIN(j + 1, size_y - 1);
	    
      uint8_t next_state = NEXT_STATE(GET_STATE(i, j));

      for (uint x = min_x; x <= max_x; x++) {
	for (uint y = min_y; y <= max_y; y++) {
	  // avoid iterating over itself
	  if (x == i && y == j) {
	    continue;
	  }
	  // update color if neighbor has next color
	  if (GET_STATE(x, y) == next_state) {
	    SET_NEXT_STATE(i, j, next_state);
	    x = max_x + 1; y = max_y + 1; 
	    break;
	  }
	}
      }
    }
  }
}

void draw_block(uint i, uint j, uint8_t color)
{
  for (uint dx = 0; dx < block_size; dx++) {
    for (uint dy = 0; dy < block_size; dy++) {
      uint x = i * block_size + dx + pad_x;
      uint y = j * block_size + dy + pad_y;
      lcdSetPixel(x, y, color);
    }
  }
}

void draw()
{
  for (uint i = 0; i < size_x; i++) {
    for (uint j = 0; j < size_y; j++) {
      draw_block(i, j, GET_NEXT_STATE(i, j));
    }
  }
  lcdDisplay();
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  lcdInit();
  ram();
  lcdClose();
  
  return 0;
}
