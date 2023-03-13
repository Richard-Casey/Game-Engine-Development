#include "Hero.h"
#include "Creature.h"
#include "Monster.h"
#include "bitmap.h"

// Constructor for Hero class
/// <summary>
/// Initializes a new instance of the <see cref="Hero"/> class.
/// </summary>
/// <param name="renderer">The renderer.</param>
/// <param name="filename">The filename.</param>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
/// <param name="name">The name.</param>
/// <param name="useTransparency">if set to <c>true</c> [use transparency].</param>
Hero::Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
    : Creature(renderer, filename, xpos, ypos, name, useTransparency)
{
    type = ObjectType::Hero;
    this->Update(); // Use update function from Hero instead of Creature
}

// Destructor for Hero class
/// <summary>
/// Finalizes an instance of the <see cref="Hero"/> class.
/// </summary>
Hero::~Hero()
{
}
