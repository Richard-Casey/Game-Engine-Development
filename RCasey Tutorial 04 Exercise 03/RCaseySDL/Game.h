#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "input.h"
#include <string>
#include "Creature.h"
#include "Monster.h"
#include "hero.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "imgui_sdl.h"
#include "imgui_internal.h"
#include "Pickup.h"
#include "MonsterHome.h"
#include <cmath>

using namespace std;


struct SDL_Window;
struct SDL_Renderer;
class Bitmap;


class Game
{
private:

	bool m_running;
	//bitmaps!
	Creature* m_monster; //04-01
	Creature* m_monsterTrans; //04-01
	Creature* m_monsterTransKeyed; //04-01
	Monster* m_pTheMonster; // Created monster
	Hero* m_pTheHero;		// created hero
	Pickup* m_Pickup;
	MonsterHome* m_MonsterHome;
	TTF_Font* m_pSmallFont;
	TTF_Font* m_pBigFont;

	void CheckEvents();
	//void UpdateText(string msg, int x, int y, TTF_Font* font, SDL_Color colour);


	Uint8 r = 127, g = 127, b = 127, a = 255;
	int xpos;
	int ypos;

	ImGuiIO* io;
	
	
public:
	SDL_Point mousePoint; //= { io->MousePos.x, io->MousePos.y };
	SDL_Rect spriteHeroRect; //= { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
	SDL_Rect spriteMonsterRect; //= { m_pTheMonster->GetX(), m_pTheMonster->GetY(), m_pTheMonster->GetW(), m_pTheMonster->GetH() };
	SDL_Rect spritePickupRect; //= { m_Pickup->GetX(), m_Pickup->GetY(), m_Pickup->GetW(), m_Pickup->GetH() };
	SDL_Window* m_Window;
	SDL_Renderer *m_Renderer;
	Game();
	~Game();
	void SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void Update();
	double  randomNumber();
	string directory = "../assets/";

	Input* input = new Input();
	
	int themonsterXpos = 10;
	int themonsterYpos = 10;

	int heroXpos = 930;
	int heroYpos = 680;

	int pickupXPos = 600;
	int pickupYPos = 600;

	int MHomeXPos = 450;
	int MHomeYPos = 450;

	bool showMonsterImgui = false;
	bool showHeroImgui = false;
	bool showPickupImgui = false;

	SDL_Rect GetSpriteHeroRect()
	{
		return spriteHeroRect;
	}

	SDL_Rect GetPickupRect()
	{
		return spritePickupRect;
	}

};

#endif
