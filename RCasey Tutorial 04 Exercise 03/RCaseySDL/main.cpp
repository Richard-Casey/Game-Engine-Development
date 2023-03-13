#include "Game.h"
#include "Profiler.h"


/// <summary>
/// Mains the specified argc.
/// </summary>
/// <param name="argc">The argc.</param>
/// <param name="argv">The argv.</param>
/// <returns></returns>
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
