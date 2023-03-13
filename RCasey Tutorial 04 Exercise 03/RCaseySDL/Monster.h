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
	/// <summary>
	/// Updates this instance.
	/// </summary>
	void Update() override
	{
		Chase();
	}

	/// <summary>
	/// The m hero to chase
	/// </summary>
	Hero* m_HeroToChase;
	/// <summary>
	/// The pickup
	/// </summary>
	Pickup* pickup;
	
};

// Draw method is not nessacry as inheriting from Creature
