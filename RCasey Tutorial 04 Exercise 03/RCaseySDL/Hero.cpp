#include "Hero.h"
#include "Creature.h"
#include "Monster.h"
#include "bitmap.h"

// Constructor for Hero class
Hero::Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
    : Creature(renderer, filename, xpos, ypos, name, useTransparency)
{
    this->Update(); // Use update function from Hero instead of Creature
}

// Destructor for Hero class
Hero::~Hero()
{
}
