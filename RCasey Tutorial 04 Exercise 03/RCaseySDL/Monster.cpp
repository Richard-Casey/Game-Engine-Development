#include "Monster.h"
#include "bitmap.h"
#include "Creature.h"
#include "Hero.h"
#include "Game.h"
#include <iostream>
#include "Pickup.h"

Monster::Monster(Hero* HeroToChase, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency) :Creature(renderer, filename, xpos, ypos, name, useTransparency)
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
			
		
		if (Game::ResetEvent != ((Uint32)-1)) {
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
			event.type = Game::ResetEvent;
			event.user.code = 1;
			event.user.data1 = 0;
			event.user.data2 = 0;
			SDL_PushEvent(&event);
		}



	}
	
}

