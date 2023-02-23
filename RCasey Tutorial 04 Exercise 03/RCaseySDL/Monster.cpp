#include "Monster.h"
#include "bitmap.h"
#include "Creature.h"
#include "Hero.h"
#include "Game.h"
#include <iostream>
#include "Pickup.h"

Monster::Monster(Hero* HeroToChase, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) :Creature(renderer, filename, xpos, ypos, useTransparency)
{
	m_HeroToChase = HeroToChase;
	this->Update();// Specifies that the update from Hero needs to be used instead of the Update from Creature
}

Monster::~Monster()
{
}

void Monster::Chase()
{
	int diffx = this->m_x - m_HeroToChase->GetX();
	int diffy = this->m_y - m_HeroToChase->GetY();
	if (diffx > 0)
		diffx = -1;
	else if(diffx < 0)
		diffx = 1;
	
	if (diffy > 0)
		diffy = -1;
	else if (diffy < 0)
		diffy = 1;

	this->m_x += diffx;
	this->m_y += diffy;

	// If Monster coords match hero coord - reset (eaten)
	if (this->m_x == m_HeroToChase->GetX() && this->m_y == m_HeroToChase->GetY())
	{
		this->m_x = 10;
		this->m_y = 10;
		m_HeroToChase->m_x = 930;
		m_HeroToChase->m_y = 680;
		
		pickup->isVisable = true;
		
	}
	
}

