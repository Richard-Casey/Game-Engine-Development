
#include "Game.h"
#include "input.h"

#undef main
int main(int argc, char* argv[])
{
	Game* game = new Game();	// create a new game object
	Input* input = new Input(); // create an Input object

	if (game && input)
	{
		// decale and init variable used for colour background
		Uint8 r = 127, g = 127, b = 127, a = 255;

		//check keys pressed to update variables
		while (!input->KeyIsPressed(KEY_ESCAPE))
		{
			input->Update();

			//increase r
			if (input->KeyIsPressed(KEY_R))
			{
				if (++r > 255) r = 0;
			}

			//increase g
			if (input->KeyIsPressed(KEY_G))
			{
				if (++g > 255) g = 0;
			}

			//increase b
			if (input->KeyIsPressed(KEY_B))
			{
				if (++b > 255) b = 0;
			}

			game->SetDisplayColour(r, g, b, a); //Set our colour display
			game->Update(); // update the game
		}

		delete input;			//clean up
		input = nullptr;

		delete game;			//clean up
		game = nullptr;
	}

	return 0;		// exit nicely
}
