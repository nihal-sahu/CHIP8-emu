#pragma once

#include <cstdint>

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

//class for rendering SDL layer
class Platform
{
	public:
		Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
		~Platform();

		//output functions
		void Update(void const* buffer, int pitch);			//updates canvas
		bool ProcessInput(uint8_t* keys);		//accepts keyboard inputs

	private:
		SDL_Window* window{};		//outputs window
		SDL_Renderer* renderer{};	//SDL renderer
		SDL_Texture* texture{};		//SDL Texture
};
