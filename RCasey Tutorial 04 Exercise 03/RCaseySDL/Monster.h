#pragma once
#include "Creature.h"
#include "bitmap.h"

class Monster : public Creature
{
public:
	Monster(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency = false);
	~Monster();
	void Update()
	{

	}
private:

};

// Draw method is not nessacry as inheriting from Creature
