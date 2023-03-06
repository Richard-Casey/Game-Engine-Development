#pragma once
#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "Monster.h"
#include "Pickup.h"
#include "Game.h"

class Monster : public Creature
{
public:
	Monster(Hero* HeroToChase, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency = false);
	~Monster();
	void Chase();
	void Update()
	{

	}

private:
	Hero* m_HeroToChase;
	Pickup* pickup;
	
};

// Draw method is not nessacry as inheriting from Creature
