
#include "Game.h"


#undef main
int main(int argc, char* argv[])
{
	Game* game = new Game();	// create a new game object
	 // create an Input object

	if (game)
	{
		// decale and init variable used for colour background
		

		//check keys pressed to update variables
		while (!game->input->KeyIsPressed(KEY_ESCAPE))
		{
			
			game->Update(); // update the game


		}

		

		delete game;			//clean up
		game = nullptr;
	}

	return 0;		// exit nicely
}


