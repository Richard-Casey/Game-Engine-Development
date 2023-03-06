#include "Game.h"

int main(int argc, char* argv[])
{
    Game* game = new Game();
    while (!game->input->KeyIsPressed(KEY_ESCAPE))
    {
        game->Update();
    }
    delete game;
    return 0;
}
