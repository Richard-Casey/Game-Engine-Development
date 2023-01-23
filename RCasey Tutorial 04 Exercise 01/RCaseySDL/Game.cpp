#include <stdio.h>
#include "Game.h"
#include "SDL.h"
#include "input.h"
#include "bitmap.h"
#include "SDL_ttf.h"
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

	m_running = true;

	m_Window = nullptr;
	m_Renderer = nullptr;


	//start up
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

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

	//read in the font
	m_pSmallFont = TTF_OpenFont("C:\\Users\\Administrator\\Desktop\\s233122\\Game Engine Development\\RCasey Tutorial 04 Exercise 01\\assets\\DejaVuSans.ttf", 15);
	m_pBigFont = TTF_OpenFont("C:\\Users\\Administrator\\Desktop\\s233122\\Game Engine Development\\RCasey Tutorial 04 Exercise 01\\assets\\DejaVuSans.ttf", 50);

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

	//free the font
	TTF_CloseFont(m_pBigFont);
	TTF_CloseFont(m_pSmallFont);
}

void Game::Update(void)
{

	CheckEvents();
	SDL_RenderClear(m_Renderer);




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

	//draw the text
	UpdateText("Small Red", 50, 10, m_pSmallFont, { 255, 0, 0 });
	UpdateText("Small Blue", 50, 40, m_pSmallFont, { 0, 0, 255 });

	char char_array[] = "Big White";
	UpdateText(char_array, 50, 140, m_pBigFont, { 255, 255, 255 });

	string myString = "Big Green";
	UpdateText(myString, 50, 70, m_pBigFont, { 0, 255, 0 });

	int testNumber = 1234;
	string testString = "Test Number: ";
	testString += to_string(testNumber);
	UpdateText(testString, 50, 210, m_pBigFont, { 255, 255, 255 });

	//show what weve drawn
	SDL_RenderPresent(m_Renderer);
	//pause for 1/60th sec (ish)
	SDL_Delay(16);
	
}

	void Game::UpdateText(string msg, int x, int y, TTF_Font* font, SDL_Color colour)
	{
		SDL_Surface* surface = nullptr;
		SDL_Texture* texture = nullptr;

		int texW = 0;
		int texH = 0;

		//SDL_Color color = { 0, 0, 0 };

		//char msg [100];
		//sprintf_s(msg, "Checks %d", m_checkTally);

		surface = TTF_RenderText_Solid(font, msg.c_str(), colour);
		if (!surface)
		{
			//surface not loaded? Output the error
			printf("[SURFACE] for font not loaded!\n");
			printf("%s\n", SDL_GetError());
		}
		else
		{
			texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
			if (!texture)
			{
				//surface not loaded? Output the error
				printf("SURFACE for font not loaded! \n");
				printf("%s\n", SDL_GetError());
			}
			else
			{
				SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
				SDL_Rect textRect = { x, y, texW, texH };

				SDL_RenderCopy(m_Renderer, texture, NULL, &textRect);
			}
		}

		if (texture)
			SDL_DestroyTexture(texture);

		if (surface)
			SDL_FreeSurface(surface);
	}

