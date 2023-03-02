#include <stdio.h>
#include <iostream>
#include <filesystem>

#include "Game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "input.h"
#include "bitmap.h"
#include "Hero.h"
#include "Monster.h"
#include <iostream>
#include "ResourceManager.h"
#include "Pickup.h"
#include <imgui.h>

void Game::SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{	

		// clean up
		// dont forget - we destroy in the REVERSE order that they were created
		if (m_Renderer)
		{
			int result = SDL_SetRenderDrawColor(
				m_Renderer,		// our target renderer
				r,				//r
				g,				//g
				b,				//b
				a				//alpha
			);
			
		}

}

void Game::UpdateText(string msg, int x, int y, TTF_Font* font, SDL_Color colour)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), colour);
	if (!surface)
	{
		printf("SURFACE for font not loaded! \n");
		printf("%s\n", SDL_GetError());
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	if (!texture)
	{
		printf("TEXTURE for font not loaded! \n");
		printf("%s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		return;
	}

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

	SDL_Rect textRect = { x, y, texW, texH };
	SDL_RenderCopy(m_Renderer, texture, NULL, &textRect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}


void Game::CheckEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != NULL)
	{

		input->Update(e);

		if (e.type == Game::ResetEvent && e.user.code == 1)
		{
			UnLoadObjects();
			LoadObjects();
		}
		if (e.type == Game::PickupEvent && e.user.code == 2)
		{
			m_Goal->isVisable = false;
		}
	}
}

double Game::randomNumber()
{
    static bool need_random = true;
    if (need_random)
    {
        srand(static_cast<unsigned int>(time(NULL)));
        need_random = false;
    }

    return static_cast<double>(rand()) / RAND_MAX;
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
		ScreenWidth,								// width, in pixels
		ScreenHeight,								// height in pixels
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

	LoadObjects();

	//DebugPrintF("System::Initialise, %d, %d, %f \n", 10, 15, 52.3f);
	Game::ResetEvent = SDL_RegisterEvents(1);
}

void Game::LoadObjects()
{
	m_pTheHero = new Hero(m_Renderer, directory + "deadpool.bmp", heroXpos, heroYpos, true);
	m_pTheMonster = new Monster(m_pTheHero, m_Renderer, directory + "GrimReaper.bmp", themonsterXpos, themonsterYpos, true);
	m_Pickup = new Pickup(m_pTheHero, m_Renderer, directory + "pickup1.bmp", pickupXPos, pickupYPos, true);
	m_Goal = new Pickup(m_pTheHero, m_Renderer, directory + "goal1.bmp", goalXPos, goalYPos, true);
	m_Goal2 = new Pickup(m_pTheHero, m_Renderer, directory + "goal2.bmp", goalXPos, goalYPos, true);

	m_pSmallFont = TTF_OpenFont("../assets/FatPixels.ttf", 25);
	m_pBigFont = TTF_OpenFont("../assets/FatPixels.ttf", 75);
	m_pInfoFont = TTF_OpenFont("../assets/Condensed.ttf", 25);
}

void Game::UnLoadObjects()
{
	delete m_pTheHero;
	delete m_pTheMonster;
	delete m_Pickup;
	delete m_Goal;
	delete m_Goal2;
	delete m_pSmallFont;
	delete m_pBigFont;

	m_pTheHero = nullptr;
	m_pTheMonster = nullptr;
	m_Pickup = nullptr;
	m_Goal = nullptr;
	m_Goal2 = nullptr;
	m_pSmallFont = nullptr;
	m_pBigFont = nullptr;
}

Game::~Game()
{
	//clean up dont forget - we destroy in the reverse order that they were created
	delete input;			
	input = nullptr;
	if (m_pTheMonster)
		delete m_pTheMonster;
	if (m_pTheHero)
		delete m_pTheHero;
	if (m_Pickup)
		delete m_Pickup;
	if (m_Goal)
		delete m_Goal;

	TTF_CloseFont(m_pBigFont);
	TTF_CloseFont(m_pSmallFont);

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
	CheckEvents();
	SDL_RenderClear(m_Renderer);

	// This starts the game in the SPASH state - essentially a menu
	if (State == Game::SPLASH)
	{
		
		SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y };
		SDL_Rect ClickScreenRect;
		ClickScreenRect.x = 0;
		ClickScreenRect.y = 0;
		ClickScreenRect.w = ScreenWidth;
		ClickScreenRect.h = ScreenHeight;
		bool isMouseOverText = SDL_PointInRect(&mousePoint, &ClickScreenRect);
		if (isMouseOverText)
		{
			printf("MOUSE ON SCREEN");
		}

		
		
		// Logic for the menu - a button press that return the state to GAME

		UpdateText("Press Enter", 210, 250, m_pBigFont, { 255,255,255 });
		UpdateText("To Start", 275, 320, m_pBigFont, { 255,255,255 });

		//UpdateText("...Or Click The Screen", 500, 550, m_pSmallFont, { 255,255,255 });

		UpdateText("Simple proof of concept game in which you retrieve the key to allow you to return to the start and complete the level", 10, 650, m_pInfoFont, { 200, 240, 236 });
		UpdateText("all while avoiding the Grim Reaper.", 350, 675, m_pInfoFont, { 223,62,62 });

		if (input->KeyIsPressed(KEY_RETURN))
		{
			State = Game::GAME;
		}

		if (input->KeyIsPressed(KEY_MOUSE))
		{
			State = Game::GAME;
		}

		
	}
	else if (State == Game::GAME)
	{
		//std::cout << randomNumber() << endl;



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


		// Show our bitmaps
		if (m_Pickup->isVisable)
			m_Pickup->draw();
		if (!m_Pickup->isVisable)
		{
			m_Goal2->draw();

		}
		m_Goal->draw();
		m_pTheMonster->draw();
		m_pTheHero->draw();	// The sequence of which the bitmaps are drawn is important
						// bitmaps drawn first are behind anything drawn after them!


		SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y };
		SDL_Rect spriteHeroRect = { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
		SDL_Rect spriteMonsterRect = { m_pTheMonster->GetX(), m_pTheMonster->GetY(), m_pTheMonster->GetW(), m_pTheMonster->GetH() };
		SDL_Rect spritePickupRect = { m_Pickup->GetX(), m_Pickup->GetY(), m_Pickup->GetW(), m_Pickup->GetH() };
		SDL_Rect spriteGoalRect = { m_Goal->GetX(), m_Goal->GetY(), m_Goal->GetW(), m_Goal->GetH() };


		// Implement pcikup update
		if (m_Pickup->isVisable)
			m_Pickup->Update();


		// Implement chase function
		m_pTheMonster->Chase();
		//handleDragAndDrop();

		//ImGui Window for Hero
		bool isMouseOverHero = SDL_PointInRect(&mousePoint, &spriteHeroRect);
		ImGui::NewFrame();
		ImGui_ImplSDL2_NewFrame(m_Window);
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		if (showHeroImgui)
		{
			ImGui::Begin("Hero");
			if (ImGui::Button("Close Window", ImVec2(100, 30)))
			{
				showHeroImgui = false;
			}
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

		// ImGui window for Monster
		bool isMouseOverMonster = SDL_PointInRect(&mousePoint, &spriteMonsterRect);
		ImGui::SetNextWindowSize(ImVec2(300, 200));

		if (showMonsterImgui)
		{
			ImGui::Begin("Monster");

			if (ImGui::Button("Close Window", ImVec2(100, 30)))
			{
				showMonsterImgui = false;
			};
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

		//IMGui window for pickup
		bool isMouseOverPickup = SDL_PointInRect(&mousePoint, &spritePickupRect);
		ImGui::SetNextWindowSize(ImVec2(300, 200));

		if (showPickupImgui)
		{
			ImGui::Begin("Pickup");

			if (ImGui::Button("Close Window", ImVec2(100, 30)))
			{
				showPickupImgui = false;
			};
			ImGui::End();
		}
		if (isMouseOverPickup && io->MouseDown[0])
		{
			showPickupImgui = true;
		}
		if (isMouseOverPickup && io->MouseReleased[0])
		{
			showPickupImgui = true;
		}

		ImGui::SetNextWindowSize(ImVec2(600, 100));
		ImGui::Begin("Game Settings");

		//converto to lfoat

		int R, G, B;
		R = r;
		G = g;
		B = b;

		ImGui::DragInt("Background Red", &R, 1.0f, 0, 255);
		ImGui::DragInt("Background Green", &G, 1.0f, 0, 255);
		ImGui::DragInt("Background Blue", &B, 1.0f, 0, 255);

		r = R;
		g = G;
		b = B;

		//unconvert

		ImGui::End();




		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());
	}
	SDL_RenderPresent(m_Renderer);
	//pause for 1/60th sec (ish)
	SDL_Delay(16);
	
}



Uint32 Game::ResetEvent = SDL_RegisterEvents(1);
Uint32 Game::PickupEvent = SDL_RegisterEvents(1);