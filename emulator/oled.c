/*
 * This file is part of the Trezor project, https://trezor.io/
 *
 * Copyright (C) 2017 Saleem Rashid <trezor@saleemrashid.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "oled.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_Texture *prokey_texture = NULL;
static SDL_Rect dstrect, prokey_srcrect;
static int ex_width = 20;
static int ex_height = 118;
SDL_MouseButtonEvent g_MouseEvent;
bool g_MouseIsDown = false;
int g_scale = 1;

#define ENV_OLED_FULLSCREEN "TREZOR_OLED_FULLSCREEN"
#define ENV_OLED_SCALE "TREZOR_OLED_SCALE"

static int emulatorFullscreen(void)
{
  const char *variable = getenv(ENV_OLED_FULLSCREEN);
  if (!variable)
  {
    return 0;
  }
  return atoi(variable);
}

static int emulatorScale(void)
{
  const char *variable = getenv(ENV_OLED_SCALE);
  if (!variable)
  {
    return 1;
  }
  int scale = atoi(variable);
  if (scale >= 1 && scale <= 16)
  {
    return scale;
  }
  return 1;
}

void oledInit(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  int scale = emulatorScale();
  g_scale = scale;
  int fullscreen = emulatorFullscreen();

  window = SDL_CreateWindow(
      "Prokey", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      (OLED_WIDTH + ex_width) * scale, (OLED_HEIGHT + ex_height) * scale,
      fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 | SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP);

  if (window == NULL)
  {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    exit(1);
  }

  // load support for the PNG image formats
  int flags = IMG_INIT_PNG;
  int initted = IMG_Init(flags);
  if ((initted & flags) != flags)
  {
    printf("IMG_Init: Failed to init required jpg and png support!\n");
    printf("IMG_Init: %s\n", IMG_GetError());
    exit(1);
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }
  if (fullscreen)
  {
    SDL_DisplayMode current_mode;
    if (SDL_GetCurrentDisplayMode(0, &current_mode) != 0)
    {
      fprintf(stderr, "Failed to get current display mode: %s\n",
              SDL_GetError());
      exit(1);
    }

    dstrect.x = (current_mode.w - OLED_WIDTH * scale) / 2;
    dstrect.y = (current_mode.h - OLED_HEIGHT * scale) / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_ShowCursor(SDL_DISABLE);
  }
  else
  {
    dstrect.x = (ex_width * scale) / 2;
    dstrect.y = (53 * scale) / 2;
  }

  dstrect.w = OLED_WIDTH * scale;
  dstrect.h = OLED_HEIGHT * scale;

  texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, OLED_WIDTH, OLED_HEIGHT);


  // load the image into ram memory using SDL_image library function
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
  SDL_Surface* prokey_surface = IMG_Load("../prokey.png");
  if (!prokey_surface)
  {
    printf("error creating surface: %s \n", SDL_GetError());
    exit(1);
  }
  // load the image data into the graphics hardware's memory
  prokey_texture = SDL_CreateTextureFromSurface(renderer, prokey_surface);
  SDL_FreeSurface(prokey_surface);
  if (!prokey_texture)
  {
    printf("error creating texture: %s\n", SDL_GetError());
    exit(1);
  }
  prokey_srcrect.x = 31;
  prokey_srcrect.y = 17;
  prokey_srcrect.w = 318;
  prokey_srcrect.h = 390;

  oledClear();
  oledRefresh();
}

void oledRefresh(void)
{
  /* Draw triangle in upper right corner */
  oledInvertDebugLink();

  const uint8_t *buffer = oledGetBuffer();

  static uint32_t data[OLED_HEIGHT][OLED_WIDTH];

  for (size_t i = 0; i < OLED_BUFSIZE; i++)
  {
    int x = (OLED_BUFSIZE - 1 - i) % OLED_WIDTH;
    int y = (OLED_BUFSIZE - 1 - i) / OLED_WIDTH * 8 + 7;

    for (uint8_t shift = 0; shift < 8; shift++, y--)
    {
      bool set = (buffer[i] >> shift) & 1;
      data[y][x] = set ? 0xFF15E6ED : 0xFF000000;
    }
  }

  SDL_UpdateTexture(texture, NULL, data, OLED_WIDTH * sizeof(uint32_t));

  SDL_RenderCopy(renderer, prokey_texture, &prokey_srcrect, NULL);
  SDL_RenderCopy(renderer, texture, NULL, &dstrect);

  SDL_RenderPresent(renderer);

  /* Return it back */
  oledInvertDebugLink();
}

void emulatorPoll(void)
{
  SDL_Event event;  

  if (SDL_PollEvent(&event))
  {
    g_MouseIsDown = false;
    if (event.type == SDL_QUIT)
    {
      exit(1);
    }
    else if (event.type == SDL_WINDOWEVENT)
    {
      switch (event.window.event)
      {
      case SDL_WINDOWEVENT_FOCUS_GAINED:
      case SDL_WINDOWEVENT_MOVED:
      case SDL_WINDOWEVENT_EXPOSED:
        oledRefresh();
        break;

      default:
        break;
      }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
      g_MouseIsDown = true;
      g_MouseEvent = event.button;
      oledRefresh();
    }    
    else if (event.type == SDL_KEYUP)
    {
      oledRefresh();
      if (event.key.keysym.scancode == SDL_SCANCODE_H)
      {
        SDL_SetWindowBordered(window, SDL_FALSE);
      }
      if (event.key.keysym.scancode == SDL_SCANCODE_K)
      {
        SDL_SetWindowBordered(window, SDL_TRUE);
      }
    }
    
  }
}
