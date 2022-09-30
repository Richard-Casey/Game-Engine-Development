#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <stdio.h>

class Game
{
private:
	
public:
	SDL_Window* m_Window;
	SDL_Renderer *m_Renderer;
	Game();
	~Game() 
	{
	
		if (m_Renderer)
		{
			SDL_DestroyRenderer(m_Renderer);
		}

		if (m_Window)
		{
			SDL_DestroyWindow(m_Window);
		}
	}
	void SetDisplayColour(void);
	
};

#endif
