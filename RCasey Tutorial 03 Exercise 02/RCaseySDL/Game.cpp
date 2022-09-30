#include "Game.h"
#include "SDL.h"



void Game::SetDisplayColour()
{	

		// clean up
		// dont forget - we destroy in the REVERSE order that they were created

	
		if (m_Renderer)
		{
			// render in a pretty red colour
			int result = SDL_SetRenderDrawColor(
				m_Renderer,		// our trarget renderer
				255,			//r
				0,				//g
				0,				//b
				255				//alpha
			);

			//wipe the display to the colour we just set.
			SDL_RenderClear(m_Renderer);

			//show what weve drawn (i.e. a red screen).
			SDL_RenderPresent(m_Renderer);

			//pause for 5 secs
			SDL_Delay(16);		//SDL_Delay takes millisecs
		}

}

Game::Game() {

	m_Window = nullptr;
	m_Renderer = nullptr;


	//start up
	SDL_Init(SDL_INIT_VIDEO);

	//create the window
	m_Window = SDL_CreateWindow(
		"My First Window",	//title
		250,				// initial x position
		50,					// initial y position
		640,				// width, in pixels
		480,				// height in pixels
		0				// window behaviour flags (ignore for now)
	);

	if (!m_Window)
	{
		printf("WINDOW initialisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}

	//now create the renderer
	m_Renderer = SDL_CreateRenderer(
		m_Window,			// link the renderer to our newly created win
		-1,					// index rendering driver (ignore for now)
		0					// renderer behaviour flags (ignore for now)
	);

	if (!m_Renderer)
	{
		printf("RENDERER intiialisation failed %s\n", SDL_GetError());
		printf("Press any key to continue\n");

		return;
	}
}

