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
	/// <summary>
	/// The filename
	/// </summary>
	struct AssetData { std::string filename; };
	/// <summary>
	/// The asset list
	/// </summary>
	std::vector<AssetData> assetList;
	/// <summary>
	/// The m bitmaps
	/// </summary>
	std::vector<Bitmap*> m_Bitmaps;
	/// <summary>
	/// The m selected bitmap
	/// </summary>
	Bitmap* m_SelectedBitmap = nullptr;
	void RenderSceneHierarchy();
	/// <summary>
	/// The objects in scene
	/// </summary>
	std::vector<Bitmap*> ObjectsInScene;
	
private:

	/// <summary>
	/// The show gravity window
	/// </summary>
	bool showGravityWindow;
	/// <summary>
	/// The m selected object
	/// </summary>
	Bitmap* m_SelectedObject = nullptr;

	/// <summary>
	/// The show selection GUI
	/// </summary>
	bool showSelectionGui;

	enum GameState
	{
		/// <summary>
		/// The game
		/// </summary>
		GAME,
		/// <summary>
		/// The splash
		/// </summary>
		SPLASH,
		/// <summary>
		/// The endgame
		/// </summary>
		ENDGAME

	};

	/// <summary>
	/// The state
	/// </summary>
	GameState State = SPLASH;

	/// <summary>
	/// The m running
	/// </summary>
	bool m_running;
	/// <summary>
	/// The m use gravity
	/// </summary>
	bool m_useGravity = true;
	
	/// <summary>
	/// The m monster
	/// </summary>
	Creature* m_monster; //04-01
	/// <summary>
	/// The m monster trans
	/// </summary>
	Creature* m_monsterTrans; //04-01
	/// <summary>
	/// The m monster trans keyed
	/// </summary>
	Creature* m_monsterTransKeyed; //04-01

	/// <summary>
	/// The m p the monster
	/// </summary>
	Monster* m_pTheMonster; // Created monster
	/// <summary>
	/// The monsters
	/// </summary>
	std::vector<Monster*> monsters; // A vector to store Monster objects
	/// <summary>
	/// The pickups
	/// </summary>
	std::vector<Pickup*> pickups; // A vector to store Pickup objects

	/// <summary>
	/// A Pointer to Hero Bitmap
	/// </summary>
	Hero* m_pTheHero;

	/// <summary>
	/// A Pointer to Pickup Bitmap
	/// </summary>
	Pickup* m_Pickup;

	/// <summary>
	/// A Pointer to Goal Bitmap
	/// </summary>
	Pickup* m_Goal;

	/// <summary>
	/// A Pointer to Goal2 Bitmap
	/// </summary>
	Pickup* m_Goal2;

	/// <summary>
	/// A Pointer to allocated Small Font
	/// </summary>
	TTF_Font* m_pSmallFont;

	/// <summary>
	/// A pointer to allocated Big Font
	/// </summary>
	TTF_Font* m_pBigFont;

	/// <summary>
	/// A Pointer to informative text font
	/// </summary>
	TTF_Font* m_pInfoFont;

	/// <summary>
	/// This is a summery of Asset manager
	/// </summary>
	void AssetManager();

	/// <summary>
	/// The CheckEvents Function looks for in game events
	/// </summary>
	void CheckEvents();

	/// <summary>
	/// Updates the text to be displayed
	/// </summary>
	/// <param name="msg">string the message to display</param>
	/// <param name="x">X Coordinate of text</param>
	/// <param name="y">Y Coordinate of text</param>
	/// <param name="font">Font Name</param>
	/// <param name="colour">Colour of font displayed</param>
	void UpdateText(std::string msg, int x, int y, TTF_Font* font, SDL_Color colour);
	
	/// <summary>
	/// Call to Render ImGui
	/// </summary>
	void Render();

	/// <summary>
	/// Call to Render Objects in scene and provide X and Y coordinate for each
	/// </summary>
	void RenderObjectsWindow();

	/// <summary>
	/// Finds and loads contents of asset folder
	/// </summary>
	void FindAssets();

	/// <summary>
	/// Vector of Bitmaps
	/// </summary>
	std::vector<Bitmap*> content;

	//const float GRAVITY = 500.0f;
	//float m_ay = 9.81f; // default value for m_ay

	/// <summary>
	/// Save Function using JSON
	/// </summary>
	void SaveWorldData();

	/// <summary>
	/// Red, Green, Blue and alpha levels (min = 0, max = 255)
	/// </summary>
	Uint8 r = 127, g = 127, b = 127, a = 255;

	/// <summary>
	/// X position in coordinates
	/// </summary>
	int xpos;

	/// <summary>
	/// Y Position in coordinates
	/// </summary>
	int ypos;

	/// <summary>
	/// Inputs and Outputs for ImGui
	/// </summary>
	ImGuiIO* io;
	
	
public:
	/// <summary>
	/// Coordinates for mouse pointer
	/// </summary>
	SDL_Point mousePoint;

	/// <summary>
	/// SDL Rectangle for Hero
	/// </summary>
	SDL_Rect spriteHeroRect;

	/// <summary>
	/// /// SDL Rectangle for Monster
	/// </summary>
	SDL_Rect spriteMonsterRect;

	/// <summary>
	/// /// SDL Rectangle for Pickup (Key)
	/// </summary>
	SDL_Rect spritePickupRect;

	/// <summary>
	/// /// SDL Rectangle for Goal
	/// </summary>
	SDL_Rect spriteGoalRect;

	/// <summary>
	/// SDL Window for Game
	/// </summary>
	SDL_Window* m_Window;

	/// <summary>
	/// SDL Renderer for window
	/// </summary>
	SDL_Renderer* m_Renderer;
	
	/// <summary>
	/// Screen Width for SDL Window
	/// </summary>
	int ScreenWidth = 1024;

	/// <summary>
	/// Screen height for SDL Window
	/// </summary>
	int ScreenHeight = 768;

	/// <summary>
	/// Game Constructor
	/// </summary>
	Game();

	/// <summary>
	/// Function for loading all game objects
	/// </summary>
	void LoadObjects();

	/// <summary>
	/// Function for unloading all game objects
	/// </summary>
	void UnLoadObjects();

	/// <summary>
	/// Game Deconstructor
	/// </summary> 
	~Game();
	
	/// <summary>
	/// Set Display Colour for SDL Window Background
	/// </summary>
	/// <param name="r">Set Red Level (0 - 255)</param>
	/// <param name="g">Set Green Level (0 - 255)</param>
	/// <param name="b">Set Blue Level (0 - 255)</param>
	/// <param name="a">Set Alpha Level (0 - 255)</param>
	void SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/// <summary>
	/// Update Function for Game
	/// </summary>
	void Update();

	/// <summary>
	/// Random number generator
	/// </summary>
	/// <returns>Returns random number</returns>
	double  randomNumber();

	/// <summary>
	/// String directory for assets folder
	/// </summary>
	std::string directory = "../assets/";

	/// <summary>
	/// Input for WASD (Hero Control) and RGB fader for background colour
	/// </summary>
	Input* input = new Input();

	/// <summary>
	/// Initial Starting X coordinate for Monster
	/// </summary>
	int themonsterXpos = 10;

	/// <summary>
	/// Initial Starting Y coordinate for Monster
	/// </summary>
	int themonsterYpos = 10;

	/// <summary>
	/// Initial Starting X coordinate for Hero
	/// </summary>
	int heroXpos = 930;

	/// <summary>
	/// Initial Starting Y coordinate for Hero
	/// </summary>
	int heroYpos = 680;

	/// <summary>
	/// Initial starting X coordinate for Pickup (Key)
	/// </summary>
	int pickupXPos = ScreenWidth / 2;

	/// <summary>
	/// Initial starting Y coordinate for Pickup (Key)
	/// </summary>
	int pickupYPos = ScreenHeight / 2;

	/// <summary>
	/// X Position of Goal
	/// </summary>
	int goalXPos = 930;

	/// <summary>
	/// Y Positiuon  of Goal
	/// </summary>
	int goalYPos = 680;

	/// <summary>
	/// Boolean to show Monster ImGui when clicked
	/// </summary>
	bool showMonsterImgui = false;
	
	/// <summary>
	/// Boolean to show Hero ImGui when clicked
	/// </summary>
	bool showHeroImgui = false;

	/// <summary>
	/// Boolean to show Pickup ImGui when clicked
	/// </summary>
	bool showPickupImgui = false;

	/// <summary>
	/// Boolean to show Goal ImGui when clicked
	/// </summary>
	bool showGoalImgui = false;

	/// <summary>
	/// Boolean to track if the Goal is active (retrusn true when key is collected)
	/// </summary>
	bool isGoalActive = false;

	/// <summary>
	/// Initialised nullptr for AssetMousDrag for Drag and drop
	/// </summary>
	Bitmap* AssetMousDrag = nullptr;
	

	/// <summary>
	/// Gets SDL_Rect coordinates for Hero
	/// </summary>
	/// <returns>
	/// Returns Hero coordinates (which are private)
	/// </returns>
	SDL_Rect GetSpriteHeroRect()
	{
		return spriteHeroRect;
	}


	/// <summary>
	/// Gets SDL_Rect coordinates for Pickup
	/// </summary>
	/// <returns>
	/// Returns Hero coordinates (which are private)
	/// </returns>
	SDL_Rect GetPickupRect()
	{
		return spritePickupRect;
	}


};

#endif
