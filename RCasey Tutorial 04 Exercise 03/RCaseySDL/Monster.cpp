#include "Monster.h"
#include "bitmap.h"
#include "Creature.h"
#include "Hero.h"
#include "Game.h"
#include <iostream>

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
}

//void Monster::caughtAndReset()
//{
//	if (monsterX && monsterY == Hero::heroX && Hero::heroY)
//	
//}
