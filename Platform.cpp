#include "Platform.h"
#include <SDL2/SDL.h>


Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
	//start the SDL window, create renderer, and create texture
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

Platform::~Platform()
{
	//closes SDL window on quit
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Platform::Update(void const* buffer, int pitch)		//updates every frame
{
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(uint8_t* keys)		//accepts keyboard input
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (event.type == SDL_KEYDOWN)		//if a key is pressed
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)		// if the escape key is pressed
			{
				quit = true;
			}
			else if (event.key.keysym.sym == SDLK_x)		//if x is pressed
			{
				keys[0] = 1;
			}
			else if (event.key.keysym.sym == SDLK_1)		//if 1 is pressed
			{
				keys[1] = 1;
			}
			else if (event.key.keysym.sym == SDLK_2)		//if 2 is pressed
			{
				keys[2] = 1;
			}
			else if (event.key.keysym.sym == SDLK_3)		//if 3 is pressed
			{
				keys[3] = 1;
			}
			else if (event.key.keysym.sym == SDLK_q)		//if q is pressed
			{
				keys[4] = 1;
			}
			else if (event.key.keysym.sym == SDLK_w)		//if w is pressed
			{
				keys[5] = 1;
			}
			else if (event.key.keysym.sym == SDLK_e)		//if e is pressed
			{
				keys[6] = 1;
			}
			else if (event.key.keysym.sym == SDLK_a)		//if a is pressed
			{
				keys[7] = 1;
			}
			else if (event.key.keysym.sym == SDLK_s)		//if s is pressed
			{
				keys[8] = 1;
			}
			else if (event.key.keysym.sym == SDLK_d)		//if d is pressed
			{
				keys[9] = 1;
			}
			else if (event.key.keysym.sym == SDLK_c)		//if c is pressed
			{
				keys[0xA] = 1;
			}
			else if (event.key.keysym.sym == SDLK_z)		//if z is pressed
			{
				keys[0xB] = 1;
			}
			else if (event.key.keysym.sym == SDLK_4)		//if 4 is pressed
			{
				keys[0xC] = 1;
			}
			else if (event.key.keysym.sym == SDLK_r)		//if r is pressed
			{
				keys[0xD] = 1;
			}
			else if (event.key.keysym.sym == SDLK_f)		//if f is pressed
			{
				keys[0xE] = 1;
			}
			else if (event.key.keysym.sym == SDLK_v)		//if v is pressed
			{
				keys[0xF] = 1;
			}
		}
		else if (event.type == SDL_KEYUP)		//if a key is released
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)		// if the escape key is released
			{
				quit = true;
			}
			else if (event.key.keysym.sym == SDLK_x)		//if x is released
			{
				keys[0] = 1;
			}
			else if (event.key.keysym.sym == SDLK_1)		//if 1 is released
			{
				keys[1] = 1;
			}
			else if (event.key.keysym.sym == SDLK_2)		//if 2 is released
			{
				keys[2] = 1;
			}
			else if (event.key.keysym.sym == SDLK_3)		//if 3 is released
			{
				keys[3] = 1;
			}
			else if (event.key.keysym.sym == SDLK_q)		//if q is released
			{
				keys[4] = 1;
			}
			else if (event.key.keysym.sym == SDLK_w)		//if w is released
			{
				keys[5] = 1;
			}
			else if (event.key.keysym.sym == SDLK_e)		//if e is released
			{
				keys[6] = 1;
			}
			else if (event.key.keysym.sym == SDLK_a)		//if a is released
			{
				keys[7] = 1;
			}
			else if (event.key.keysym.sym == SDLK_s)		//if s is released
			{
				keys[8] = 1;
			}
			else if (event.key.keysym.sym == SDLK_d)		//if d is released
			{
				keys[9] = 1;
			}
			else if (event.key.keysym.sym == SDLK_c)		//if c is released
			{
				keys[0xA] = 1;
			}
			else if (event.key.keysym.sym == SDLK_z)		//if z is released
			{
				keys[0xB] = 1;
			}
			else if (event.key.keysym.sym == SDLK_4)		//if 4 is released
			{
				keys[0xC] = 1;
			}
			else if (event.key.keysym.sym == SDLK_r)		//if r is released
			{
				keys[0xD] = 1;
			}
			else if (event.key.keysym.sym == SDLK_f)		//if f is released
			{
				keys[0xE] = 1;
			}
			else if (event.key.keysym.sym == SDLK_v)		//if v is released
			{
				keys[0xF] = 1;
			}
		}
	}
	
	return quit;
}

