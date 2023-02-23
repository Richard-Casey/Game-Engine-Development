#pragma once
#include "bitmap.h"
#include "SDL.h"
#include "Monster.h"

class MonsterHome : public Bitmap
{
public:
	MonsterHome(Monster* monster, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency);
	Monster* m_monster;
};

