#pragma once
#include "Creature.h"
#include "Hero.h"
#include "bitmap.h"

class Hero : public Creature
{
public:
	Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency);
	~Hero();
	void Update()
	{

	}
	void Eaten();


private:
	

};
