#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <imgui.h>

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


void Game::SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (m_Renderer)
	{
		SDL_SetRenderDrawColor(m_Renderer, r, g, b, a);
	}
}


void Game::UpdateText(string msg, int x, int y, TTF_Font* font, SDL_Color colour)
{
	// render the text surface
	SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), colour);
	if (!surface)
	{
		printf("Error: Unable to render text surface! \n");
		return;
	}

	// create texture from the surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	SDL_FreeSurface(surface); // free the surface after creating texture
	if (!texture)
	{
		printf("Error: Unable to create texture from rendered text! \n");
		return;
	}

	// get the dimensions of the texture
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

	// render the texture to the screen
	SDL_Rect textRect = { x, y, texW, texH };
	SDL_RenderCopy(m_Renderer, texture, NULL, &textRect);

	// free the texture
	SDL_DestroyTexture(texture);
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
	m_pSmallFont = TTF_OpenFont("../assets/FatPixels.ttf", 25);
	m_pBigFont = TTF_OpenFont("../assets/FatPixels.ttf", 75);
	m_pInfoFont = TTF_OpenFont("../assets/Condensed.ttf", 25);


	Game::ResetEvent = SDL_RegisterEvents(1);


	// Asset Editor Code Provided by Nick
	std::vector<Bitmap*> content;
	std::string path = "../Assets";
	for (const auto& entry : std::filesystem::directory_iterator(path)) //directory_iterator(path) //recursive_
	{
		if (entry.path().extension() == ".bmp" || entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
		{
			Bitmap* Asset = new Bitmap(m_Renderer, entry.path().string(), 0, 0, entry.path().string(),  true);
			content.push_back(Asset);

		}
		else if (entry.is_directory())
		{
			std::cout << "dir " << entry << std::endl;
		}
		//debug
		std::cout << entry.path() << std::endl;
	}

}

void Game::LoadObjects()
{
	m_pTheHero = new Hero(m_Renderer, directory + "deadpool.bmp", heroXpos, heroYpos, "Hero", true);
	m_pTheMonster = new Monster(m_pTheHero, m_Renderer, directory + "GrimReaper.bmp", themonsterXpos, themonsterYpos,"Monster", true);
	m_Pickup = new Pickup(m_pTheHero, m_Renderer, directory + "pickup1.bmp", pickupXPos, pickupYPos, "PickUp", true);
	m_Goal = new Pickup(m_pTheHero, m_Renderer, directory + "goal1.bmp", goalXPos, goalYPos,"goal", true);
	m_Goal2 = new Pickup(m_pTheHero, m_Renderer, directory + "goal2.bmp", goalXPos, goalYPos, "gloal2", true);

	ObjectsInScene.clear();
	ObjectsInScene.push_back(m_pTheHero);
	ObjectsInScene.push_back(m_pTheMonster);
	ObjectsInScene.push_back(m_Pickup);
	ObjectsInScene.push_back(m_Goal);
	ObjectsInScene.push_back(m_Goal2);
}

void Game::UnLoadObjects()
{
	delete m_pTheHero;
	delete m_pTheMonster;
	delete m_Pickup;
	delete m_Goal;
	delete m_Goal2;
	//delete m_pSmallFont;
	//delete m_pBigFont;

	m_pTheHero = nullptr;
	m_pTheMonster = nullptr;
	m_Pickup = nullptr;
	m_Goal = nullptr;
	m_Goal2 = nullptr;
	//m_pSmallFont = nullptr;
	//m_pBigFont = nullptr;
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
	ImGui::NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	// This starts the game in the SPASH state - essentially a menu
	if (State == Game::SPLASH)
	{
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
		SDL_Rect spriteGoal2Rect = { m_Goal2->GetX(), m_Goal2->GetY(), m_Goal2->GetW(), m_Goal2->GetH() };


		// Implement pcikup update
		if (m_Pickup->isVisable)
			m_Pickup->Update();


		// Implement chase function
		m_pTheMonster->Chase();
		RenderObjectsWindow();



		//ImGui Window for Hero
		bool isMouseOverHero = SDL_PointInRect(&mousePoint, &spriteHeroRect);
		
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		if (showHeroImgui)
		{
			ImGui::Begin("Hero");
			
			bool checkboxValue = applyGravityHero;
			if (ImGui::Checkbox("Apply Gravity", &checkboxValue))
			{
				applyGravityHero = checkboxValue;
			}
			

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

			bool checkboxValue = applyGravityMonster;
			if (ImGui::Checkbox("Apply Gravity", &checkboxValue))
			{
				applyGravityMonster = checkboxValue;
			}

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

			bool checkboxValue = applyGravityPickup;
			if (ImGui::Checkbox("Apply Gravity", &checkboxValue))
			{
				applyGravityPickup = checkboxValue;
			}

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


		AssetManager();
		RenderSceneHierarchy();

		
	}

	if (applyGravityHero)
	{
		m_pTheHero->addoffset(0, 2); // add a constant value to the Y coordinate
	}
	if (applyGravityMonster)
	{
		m_pTheMonster->addoffset(0, 2);
	}
	if (applyGravityPickup)
	{
		m_Pickup->addoffset(0, 2);
	}
	if (applyGravityGoal)
	{
		m_Goal->addoffset(0, 2);
	}
	if (applyGravityGoal2)
	{
		m_Goal2->addoffset(0, 2);
	}

	SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y };
	SDL_Rect spriteHeroRect = { m_pTheHero->GetX(), m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
	bool isMouseOverHero = SDL_PointInRect(&mousePoint, &spriteHeroRect);
	if (isMouseOverHero && io->MouseDown[0])
	{
		showGravityWindow = true;
	}


	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
	//pause for 1/60th sec (ish)
	SDL_Delay(16);
	
}

void Game::RenderSceneHierarchy()
{
	ImGui::SetNextWindowSize(ImVec2(300, 200));
	ImGui::Begin("Scene Hierarchy");
	// Window content here
	for (int i = 0; i < ObjectsInScene.size(); i++)
	{

		// Add scene hierarchy nodes here
		if (ImGui::TreeNode(ObjectsInScene[i]->GetName().c_str()))
		{
			// Add child nodes here
			ImGui::TreePop();
		}
	}

	ImGui::End();
}


void Game::MoveObject(SDL_Rect& rect)
{
	ImGuiIO& io = ImGui::GetIO();
	float speed = 100.0f * io.DeltaTime;

	if (ImGui::IsKeyDown(SDL_SCANCODE_UP))
	{
		rect.y -= (int)speed;
	}

	if (ImGui::IsKeyDown(SDL_SCANCODE_DOWN))
	{
		rect.y += (int)speed;
	}

	if (ImGui::IsKeyDown(SDL_SCANCODE_LEFT))
	{
		rect.x -= (int)speed;
	}

	if (ImGui::IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		rect.x += (int)speed;
	}
}

void Game::RenderObjectsWindow()
{
	//ImGui::NewFrame();
	ImGui::Begin("Objects");

	if (ImGui::Button("Select Hero"))
	{
		m_SelectedObject = m_pTheHero;
	}

	if (ImGui::Button("Select Monster"))
	{
		m_SelectedObject = m_pTheMonster;
	}

	if (ImGui::Button("Select Pickup"))
	{
		m_SelectedObject = m_Pickup;
	
	}

	if (ImGui::Button("Select Goal"))
	{
		m_SelectedObject = m_Goal;
	}

	ImGui::End();

	if (m_SelectedObject)
	{
		ImGui::Begin("Object Details");

		int x = m_SelectedObject->m_x;
		int y = m_SelectedObject->m_y;

		ImGui::InputInt("X", &x);
		ImGui::InputInt("Y", &y);

		m_SelectedObject->SetPosition(x, y);

		ImGui::End();
	}
	//ImGui::EndFrame();
}


void Game::AssetManager()
{
	ImGui::Begin("Asset Manager");
	if (ImGui::Button("Refresh"))
	{
		// Reload the asset folder
	}

	for (const auto& file : std::filesystem::directory_iterator("../assets"))
	{
		if (ImGui::Selectable(file.path().string().c_str()))
		{
			// set the drag and drop payload when a file is selected
			//ImGui::BeginDragDropSource();
			//ImGui::SetDragDropPayload("ASSET", file.path().string().c_str(), sizeof(char) * file.path().string().size(), ImGuiCond_Once);
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
		{
			// Get the dropped asset path
			std::string assetPath(reinterpret_cast<const char*>(payload->Data), payload->DataSize);

			// Load asset into the game
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();
}




Uint32 Game::ResetEvent = SDL_RegisterEvents(1);
Uint32 Game::PickupEvent = SDL_RegisterEvents(1);
