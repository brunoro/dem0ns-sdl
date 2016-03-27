#include <SDL.h>

#define BTN_NONE 0
#define BTN_UP 1
#define BTN_DOWN 2
#define BTN_LEFT 3
#define BTN_RIGHT 4
#define BTN_ENTER 5

uint8_t encode_keysym(SDLKey sym)
{
  switch (sym) {
  case SDLK_UP:
    return BTN_UP;
  case SDLK_DOWN:
    return BTN_DOWN;
  case SDLK_LEFT:
    return BTN_LEFT;
  case SDLK_RIGHT:
    return BTN_RIGHT;
  case SDLK_RETURN:
    return BTN_ENTER;
  default:
    return BTN_NONE;
  }
}

uint8_t getInputRaw()
{
  SDL_Event event;
  uint8_t key;
  while (SDL_PollEvent(&event)) {
    switch (event.type){
    case SDL_KEYDOWN:
      key = encode_keysym(event.key.keysym.sym);
      if (key != BTN_NONE) return key;
      break;
    default:
      break;
    }
  }
  return BTN_NONE;
}

