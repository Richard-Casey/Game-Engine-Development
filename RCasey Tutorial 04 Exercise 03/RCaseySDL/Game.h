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




//using namespace std;


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
	std::vector<Bitmap*> m_Bitmaps;
	Bitmap* m_SelectedBitmap = nullptr;
	void RenderSceneHierarchy();
	std::vector<Bitmap*> ObjectsInScene;
private:

	bool showGravityWindow;
	Bitmap* m_SelectedObject = nullptr;

	bool showSelectionGui;

	enum GameState
	{
		GAME,
		SPLASH,
		ENDGAME

	};

	GameState State = SPLASH;

	bool m_running;
	bool m_useGravity = true;
	//bitmaps!
	Creature* m_monster; //04-01
	Creature* m_monsterTrans; //04-01
	Creature* m_monsterTransKeyed; //04-01

	Monster* m_pTheMonster; // Created monster
	std::vector<Monster*> monsters; // A vector to store Monster objects
	std::vector<Pickup*> pickups; // A vector to store Pickup objects

	
	Hero* m_pTheHero;		// created hero
	Pickup* m_Pickup;
	Pickup* m_Goal;
	Pickup* m_Goal2;
	TTF_Font* m_pSmallFont;
	TTF_Font* m_pBigFont;
	TTF_Font* m_pInfoFont;

	/// <summary>
	/// this is a summery of Asset manager
	/// </summary>
	void AssetManager();
	void CheckEvents();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="msg">string the message to display</param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="font"></param>
	/// <param name="colour"></param>
	void UpdateText(std::string msg, int x, int y, TTF_Font* font, SDL_Color colour);
	void Render();
	void RenderObjectsWindow();
	void MoveObject(SDL_Rect& rect);
	

	void FindAssets();
	std::vector<Bitmap*> content;

	const float GRAVITY = 500.0f;
	float m_ay = 9.81f; // default value for m_ay


	void SaveWorldData();

	Uint8 r = 127, g = 127, b = 127, a = 255;
	int xpos;
	int ypos;

	ImGuiIO* io;
	
	
public:
	SDL_Point mousePoint; 
	SDL_Rect spriteHeroRect; 
	SDL_Rect spriteMonsterRect; 
	SDL_Rect spritePickupRect; 
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
	std::string directory = "../assets/";

	Input* input = new Input();

	
	int themonsterXpos = 10;
	int themonsterYpos = 10;

	int heroXpos = 930;
	int heroYpos = 680;

	int pickupXPos = ScreenWidth / 2;
	int pickupYPos = ScreenHeight / 2;

	int MHomeXPos = 450;
	int MHomeYPos = 450;

	int goalXPos = 930;
	int goalYPos = 680;


	bool showMonsterImgui = false;
	bool showHeroImgui = false;
	bool showPickupImgui = false;
	bool showGoalImgui = false;
	bool isGoalActive = false;
	Bitmap* AssetMousDrag = nullptr;
	

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
