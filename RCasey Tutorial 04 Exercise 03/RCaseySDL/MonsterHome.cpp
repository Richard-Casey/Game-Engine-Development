#include "MonsterHome.h"
#include "bitmap.h"
#include "Hero.h"
#include "Game.h"

MonsterHome::MonsterHome(Monster* monster, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) : Bitmap(renderer, filename, xpos, ypos, useTransparency)
{
	m_monster = monster;
};
