#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "Pickup.h"
#include "Game.h"

Pickup::Pickup(Hero* hero, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) : Bitmap(renderer, filename, xpos, ypos, useTransparency)
{
	m_hero = hero;
	//this->Update(); // Specifies that the update from Pickup needs to be used instead of the Update from Creature or Hero
}

//Pickup::~Pickup()
//{
//}

void Pickup::Update()
{
	SDL_Point heroPos{ m_hero->GetX() + (m_hero->GetW() / 2), m_hero->GetY() + (m_hero->GetH() / 2) };
	SDL_Rect spritePickupRect = { GetX(), GetY(), GetW(), GetH() };

	if (SDL_PointInRect(&heroPos, &spritePickupRect))
	{
		isVisable = false;
		printf("Item collected\n");
	}
}
