#include <stdio.h>
#include "Game.h"
#include "SDL.h"
#include "input.h"
#include "bitmap.h"
#include "SDL_ttf.h"
#include "Hero.h"
#include "Monster.h"
#include <iostream>




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
				g,				//g
				b,				//b
				a				//alpha
			);
			
		}

}

void Game::CheckEvents()
{
}

Game::Game() 
{

	m_running = true;
	m_Window = nullptr;
	m_Renderer = nullptr;


	//start up
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	//create the window
	m_Window = SDL_CreateWindow(
		"My First Window",					//title
		SDL_WINDOWPOS_CENTERED,				// initial x position
		SDL_WINDOWPOS_CENTERED,				// initial y position
		1024,								// width, in pixels
		768,								// height in pixels
		SDL_WINDOW_RESIZABLE);		// window behaviour flags (ignore for now) - can be OR'd together using ||


		SDL_GLContext SDL_GLContext = SDL_GL_CreateContext(m_Window);
		
	

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

	

	//imGUI Setup THIS NEEDS TO BE AFTER THE RENDERER
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	ImGuiSDL::Initialize(m_Renderer, DisplayMode.w, DisplayMode.h);

	io= &ImGui::GetIO();
	(void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GLContext);

	// Find mouse coords and store in variables
	

	



	
	m_monsterTransKeyed = new Creature(m_Renderer, directory + "monsterTrans.bmp",
										300, 100, true);

	
	m_pTheHero = new Hero(m_Renderer, directory + "deadpool.bmp", 
							heroXpos, heroYpos, true);
	m_pTheMonster = new Monster(m_pTheHero , m_Renderer, directory + "GrimReaper.bmp", 
							themonsterXpos, themonsterYpos, true);



}

Game::~Game()
{
	delete input;			//clean up
	input = nullptr;
	
	//clean up
//dont forget - we destroy in the reverse order that they were created

//Destroy the bitmaps.
	if (m_pTheMonster)
		delete m_pTheMonster;

	if (m_monsterTransKeyed)
		delete m_monsterTransKeyed;

	if (m_monsterTrans)
		delete m_monsterTrans;

	if (m_monster)
		delete m_monster;

	if (m_pTheHero)
		delete m_pTheHero;

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
		m_pTheHero->addoffset(0, -2);
	}

	if (input->KeyIsPressed(KEY_S))
	{
		m_pTheHero->addoffset(0, 2);
	}

	if (input->KeyIsPressed(KEY_A))
	{
		m_pTheHero->addoffset(-2, 0);
	}

	if (input->KeyIsPressed(KEY_D))
	{
		m_pTheHero->addoffset(2, 0);
	}

	SetDisplayColour(r, g, b, a); //Set our colour display
	//wipe the display to the currently set colour.

	// Implement chase function
	m_pTheMonster->Chase();

	// Show our bitmaps
	m_pTheMonster->draw();
	m_pTheHero->draw();	// The sequence of which the bitmaps are drawn is important
						// bitmaps drawn first are behind anything drawn after them!



	SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y};
	SDL_Rect spriteHeroRect = { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
	SDL_Rect spriteMonsterRect = {m_pTheMonster->GetX(), m_pTheMonster->GetY(), m_pTheMonster->GetW(), m_pTheMonster->GetH() };


	bool isMouseOverHero = SDL_PointInRect(&mousePoint, &spriteHeroRect);
	ImGui::NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	if (showHeroImgui)
	{
		ImGui::Begin("Hero");
		ImGui::
		ImGui::End();
	}
	if (isMouseOverHero && io->MouseDown[0])
	{	
		showHeroImgui = true;
	}
	if (isMouseOverHero && io->MouseReleased[0])
	{
		showHeroImgui = true;
	}
	
	

	bool isMouseOverMonster = SDL_PointInRect(&mousePoint, &spriteMonsterRect);
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	
	if (showMonsterImgui)
	{
		ImGui::Begin("Monster");
		ImGui::End();
	}
	if (isMouseOverMonster && io->MouseDown[0])
	{
		showMonsterImgui = true;
	}
	if (isMouseOverMonster && io->MouseReleased[0])
	{
		showMonsterImgui = true;
	}
	
		

	// imGUI input must be between here and "ImGUI::Render"
	 
	 
	//ImGui::ShowDemoWindow(nullptr);
	
	
	// Every new Window for ImGUI must start with ImGui::Begin ("Name Window") and end with ImGui::End


	// End ImGUI Input
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	
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

