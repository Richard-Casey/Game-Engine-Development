#include "Game.h"
#include "Profiler.h"


int main(int argc, char* argv[])
{
    
    Game* game = new Game();
    while (!game->input->KeyIsPressed(KEY_ESCAPE))
    {
        Profile p("System Update");
        game->Update();
    }
    delete game;
    return 0;
}
