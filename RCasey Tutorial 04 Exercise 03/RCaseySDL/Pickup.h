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
	
	/// <summary>
	/// The m hero
	/// </summary>
	Hero* m_hero;
	/// <summary>
	/// The directory
	/// </summary>
	std::string directory = "../assets/";
	//int m_x, m_y;
};


