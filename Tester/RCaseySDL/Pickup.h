#pragma once
#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "SDL.h"


class Pickup : public Bitmap
{
public:
	Pickup(Hero* hero, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency);
	void Update();
	bool isVisable = true;
	Hero* m_hero;
	std::string directory = "../assets/";
	//int m_x, m_y;
	void draw()
	{
		if (isVisable)
			Bitmap::draw();
	}
};


