#pragma once
#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "Monster.h"

class Monster : public Creature
{
public:
	Monster(Hero* HeroToChase, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency = false);
	~Monster();
	void Chase();
	void Update()
	{

	}
	void caughtAndReset();
private:
	Hero* m_HeroToChase;

};

// Draw method is not nessacry as inheriting from Creature
