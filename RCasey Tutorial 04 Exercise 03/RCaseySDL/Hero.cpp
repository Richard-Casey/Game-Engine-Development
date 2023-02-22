#include "Hero.h"
#include "bitmap.h"
#include "Creature.h"
#include "Monster.h"

Hero::Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) :Creature(renderer, filename, xpos, ypos, useTransparency)
{
	this->Update(); // Specifies that the update from Hero needs to be used instead of the Update from Creature
}

Hero::~Hero()
{
}



