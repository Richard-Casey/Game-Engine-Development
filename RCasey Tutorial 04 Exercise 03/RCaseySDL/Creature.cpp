#include "Creature.h"
#include "bitmap.h"

//bitmap
//draw
//update
Creature::Creature(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency): Bitmap(renderer, filename, xpos, ypos, useTransparency)
{
}

Creature::~Creature()
{
}
