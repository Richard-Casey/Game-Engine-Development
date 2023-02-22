#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "Pickup.h"
#include "Game.h"

Pickup::Pickup(Hero* hero, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) : Bitmap(renderer, filename, xpos, ypos, useTransparency)
{
	m_hero = hero;
	this->Update(); // Specifies that the update from Pickup needs to be used instead of the Update from Creature or Hero
}

Pickup::~Pickup()
{
}

void Pickup::Update()
{
	if (this->m_x == m_hero->GetX() && this->m_y == m_hero->GetY())
	{
		remove("../assets/pickup.bmp");
		isVisable = false;
		printf("Item collected");
	
	}
}