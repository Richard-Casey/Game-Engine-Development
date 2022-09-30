#include "SDL.h"
#undef main

void main(void)
{
	//start up
	SDL_Init(SDL_INIT_EVERYTHING);

	//clean up
	SDL_Quit();
}