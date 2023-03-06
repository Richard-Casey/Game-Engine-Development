#include "Hero.h"
#include "bitmap.h"
#include "Creature.h"
#include "Monster.h"

Hero::Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos,  std::string name, bool useTransparency) :Creature(renderer, filename, xpos, ypos, name, useTransparency)
{
	this->Update(); // Specifies that the update from Hero needs to be used instead of the Update from Creature
}

Hero::~Hero()
{
}



