#include <stdio.h>
#include "Game.h"
#include "SDL.h"
#include "input.h"
#include "bitmap.h"
using namespace std;



void Game::SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{	

		// clean up
		// dont forget - we destroy in the REVERSE order that they were created

	
		if (m_Renderer)
		{
			// render in a pretty red colour
			int result = SDL_SetRenderDrawColor(
				m_Renderer,		// our target renderer
				r,				//r
				g,			//g
				b,			//b
				a				//alpha
			);

			//wipe the display to the colour we just set.
			//SDL_RenderClear(m_Renderer);

			//show what weve drawn (i.e. a red screen).
			//SDL_RenderPresent(m_Renderer);

			//pause for 5 secs
			//SDL_Delay(16);		//SDL_Delay takes millisecs

			
		}

}



void Game::CheckEvents()
{
}

Game::Game() {

	m_Window = nullptr;
	m_Renderer = nullptr;


	//start up
	SDL_Init(SDL_INIT_VIDEO);

	//create the window
	m_Window = SDL_CreateWindow(
		"My First Window",	//title
		600,				// initial x position
		300,				// initial y position
		640,				// width, in pixels
		480,				// height in pixels
		0				// window behaviour flags (ignore for now)
	);

	if (!m_Window)
	{
		printf("WINDOW initialisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}

	//now create the renderer
	m_Renderer = SDL_CreateRenderer(
		m_Window,			// link the renderer to our newly created win
		-1,					// index rendering driver (ignore for now)
		0					// renderer behaviour flags (ignore for now)
	);

	if (!m_Renderer)
	{
		printf("RENDERER intiialisation failed %s\n", SDL_GetError());
		printf("Press any key to continue\n");

		return;
	}

	std::string directory1 = "C:\\Users\\Administrator\\Desktop\\s233122\\Game Engine Development\\RCasey Tutorial 03 Exercise 03\\assets\\";
	std::string directory2 = "C:\\Users\\riche\\OneDrive\\Desktop\\s233122\\Game-Engine-Development\\RCasey Tutorial 04 Exercise 01\\assets\\";
	float monsterxpos = 200;
	float monsterypos = 100;
	m_monster = new Bitmap(m_Renderer, directory2 + "monster.bmp", 100, 100);
	m_monsterTrans = new Bitmap(m_Renderer, directory2 + "monsterTrans.bmp", monsterxpos, monsterypos);
	m_monsterTransKeyed = new Bitmap(m_Renderer, directory2 + "monsterTrans.bmp", 300, 100, true);

	//Update();
}

Game::~Game()
{


	delete input;			//clean up
	input = nullptr;
	

	//clean up
//dont forget - we destroy in the reverse order that they were created

//Destroy the bitmaps.
	if (m_monsterTransKeyed)
		delete m_monsterTransKeyed;

	if (m_monsterTrans)
		delete m_monsterTrans;

	if (m_monster)
		delete m_monster;

	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);
	}

	if (m_Window)
	{
		SDL_DestroyWindow(m_Window);
	}
}

void Game::Update(void)
{
	SDL_RenderClear(m_Renderer);
	CheckEvents();





	input->Update();

	//increase r
	if (input->KeyIsPressed(KEY_R))
	{
		if (++r > 255) r = 0;
	}

	//increase g
	if (input->KeyIsPressed(KEY_G))
	{
		if (++g > 255) g = 0;
	}

	//increase b
	if (input->KeyIsPressed(KEY_B))
	{
		if (++b > 255) b = 0;
	}

	if (input->KeyIsPressed(KEY_W))
	{
		m_monsterTrans->addoffset(0, -1);
	}

	if (input->KeyIsPressed(KEY_S))
	{
		m_monsterTrans->addoffset(0, 1);
	}

	if (input->KeyIsPressed(KEY_A))
	{
		m_monsterTrans->addoffset(-1, 0);
	}

	if (input->KeyIsPressed(KEY_D))
	{
		m_monsterTrans->addoffset(1, 0);
	}


	SetDisplayColour(r, g, b, a); //Set our colour display
	//wipe the display to the currently set colour.

	//show our bitmaps
	m_monster->draw();
	m_monsterTrans->draw();
	m_monsterTransKeyed->draw();

	//show what weve drawn
	SDL_RenderPresent(m_Renderer);
	SDL_Delay(16);
	//pause for 1/60th sec (ish)

}