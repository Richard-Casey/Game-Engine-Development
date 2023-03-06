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
#include <cmath>
#include <vector>
#include <unordered_map>
#include <filesystem>


using namespace std;


struct SDL_Window;
struct SDL_Renderer;
class Bitmap;
class Monster;


class Game
{
public :
	static Uint32 ResetEvent;
	static Uint32 PickupEvent;
	struct AssetData { std::string filename; };
	std::vector<AssetData> assetList;
	void RenderSceneHierarchy();
private:
	bool applyGravityHero;
	bool applyGravityMonster;
	bool applyGravityPickup;
	bool applyGravityGoal;
	bool applyGravityGoal2;
	bool showGravityWindow;
	Bitmap* m_SelectedObject = nullptr;

	enum GameState
	{
		GAME,
		SPLASH

	};

	GameState State = SPLASH;

	bool m_running;
	bool m_useGravity = true;
	//bitmaps!
	Creature* m_monster; //04-01
	Creature* m_monsterTrans; //04-01
	Creature* m_monsterTransKeyed; //04-01

	Monster* m_pTheMonster; // Created monster


	vector<Bitmap*> ObjectsInScene;
	Hero* m_pTheHero;		// created hero
	Pickup* m_Pickup;
	Pickup* m_Goal;
	Pickup* m_Goal2;
	TTF_Font* m_pSmallFont;
	TTF_Font* m_pBigFont;
	TTF_Font* m_pInfoFont;

	void AssetManager();
	void CheckEvents();
	void UpdateText(string msg, int x, int y, TTF_Font* font, SDL_Color colour);
	void Render();
	void RenderObjectsWindow();
	void MoveObject(SDL_Rect& rect);
	
	const float GRAVITY = 500.0f;
	float m_ay = 9.81f; // default value for m_ay


	Uint8 r = 127, g = 127, b = 127, a = 255;
	int xpos;
	int ypos;

	ImGuiIO* io;
	
	
public:
	SDL_Point mousePoint; //= { io->MousePos.x, io->MousePos.y };
	SDL_Rect spriteHeroRect; //= { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
	SDL_Rect spriteMonsterRect; //= { m_pTheMonster->GetX(), m_pTheMonster->GetY(), m_pTheMonster->GetW(), m_pTheMonster->GetH() };
	SDL_Rect spritePickupRect; //= { m_Pickup->GetX(), m_Pickup->GetY(), m_Pickup->GetW(), m_Pickup->GetH() };
	SDL_Rect spriteGoalRect;
	SDL_Window* m_Window;
	SDL_Renderer *m_Renderer;
	int ScreenWidth = 1024;
	int ScreenHeight = 768;
	Game();
	void LoadObjects();
	void UnLoadObjects();
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

	int goalXPos = 930;
	int goalYPos = 680;


	bool showMonsterImgui = false;
	bool showHeroImgui = false;
	bool showPickupImgui = false;
	bool showGoalImgui = false;
	bool isGoalActive = false;
	

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
