#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"

/// <summary>
/// Initializes a new instance of the <see cref="Creature"/> class.
/// </summary>
/// <param name="renderer">The renderer.</param>
/// <param name="filename">The filename.</param>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
/// <param name="name">The name.</param>
/// <param name="useTransparency">if set to <c>true</c> [use transparency].</param>
Creature::Creature(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
    : Bitmap(renderer, filename, xpos, ypos, name, useTransparency)
{
    // constructor body
}

/// <summary>
/// Finalizes an instance of the <see cref="Creature"/> class.
/// </summary>
Creature::~Creature()
{
    // destructor body
}
