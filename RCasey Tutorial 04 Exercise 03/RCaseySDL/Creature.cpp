#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"

Creature::Creature(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
    : Bitmap(renderer, filename, xpos, ypos, name, useTransparency)
{
    // constructor body
}

Creature::~Creature()
{
    // destructor body
}
