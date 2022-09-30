#undef main
#include "Game.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
	Game* game = new Game();	// create a new game object

	if (game)
	{
		game->SetDisplayColour();	// set and show our coloured display
		delete game;
		game = nullptr;
	}

	return 0;		// exit nicely
}
