#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <imgui.h>

#include <fstream>

#include "nlohmann/json.hpp"

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

using json = nlohmann::json;

/*
[
{
	type: 0,
	x: 300,
	y: 400,
	file: "spritedata.bmp"
},
{
	type: 0,
	x: 300,
	y: 400,
	file: "spritedata.bmp"
}
]
*/

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
			//m_Goal->isVisable = false;
			m_Goal2->isVisable = true;
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

	FindAssets();

	Game::ResetEvent = SDL_RegisterEvents(1);
}

void to_json(json& j, const Bitmap& p) {
	j = json{
		{"type", (int)p.type},
		{"x", p.m_x},
		{"y", p.m_y},
		{"file", p.path}
	};
}

void from_json(const json& j, Bitmap& p) {
	j.at("type").get_to(p.type);
	j.at("x").get_to(p.m_x);
	j.at("y").get_to(p.m_y);
	j.at("file").get_to(p.path);
}

void Game::LoadObjects()
{
	/*m_pTheHero = new Hero(m_Renderer, directory + "deadpool.bmp", heroXpos, heroYpos, "Hero", true);
	m_pTheMonster = new Monster(m_pTheHero, m_Renderer, directory + "GrimReaper.bmp", themonsterXpos, themonsterYpos,"Monster", true);
	m_Pickup = new Pickup(m_pTheHero, m_Renderer, directory + "pickup1.bmp", pickupXPos, pickupYPos, "PickUp", true);
	m_Goal = new Pickup(m_pTheHero, m_Renderer, directory + "goal1.bmp", goalXPos, goalYPos,"goal", true);
	m_Goal2 = new Pickup(m_pTheHero, m_Renderer, directory + "goal2.bmp", goalXPos, goalYPos, "gloal2", true);
	m_Goal2->isVisable = false;

	ObjectsInScene.clear();
	ObjectsInScene.push_back(m_pTheHero);
	ObjectsInScene.push_back(m_pTheMonster);
	ObjectsInScene.push_back(m_Pickup);
	ObjectsInScene.push_back(m_Goal);
	ObjectsInScene.push_back(m_Goal2);*/

	std::vector<Monster*> monsters;
	std::vector<Pickup*> pickups;

	std::ifstream f("../assets/world.json");
	if (f.good())
	{
		json data = json::parse(f);

		for (auto obj : data)
		{
			Bitmap* newObject = nullptr;

			ObjectType type = obj.at("type");
			std::string file = obj.at("file");
			int x = obj.at("x");
			int y = obj.at("y");

			switch (type)
			{
			case ObjectType::Hero:
				newObject = new Hero(m_Renderer, file, x, y, "Hero", true);
				m_pTheHero = static_cast<Hero*>(newObject);
				break;
			case ObjectType::Monster:
				newObject = new Monster(nullptr, m_Renderer, file, x, y, "Monster", true);
				monsters.push_back(static_cast<Monster*>(newObject));
				break;
			case ObjectType::Pickup:
			{
				bool endGoal = obj.at("end");
				newObject = new Pickup(nullptr, m_Renderer, file, x, y, "Pickup", true);
				if (endGoal)
				{
					m_Goal2 = static_cast<Pickup*>(newObject);
					m_Goal2->isEnd = true;
				}
				pickups.push_back(static_cast<Pickup*>(newObject));
				break;
			}
			case ObjectType::Static:
				newObject = new Bitmap(m_Renderer, file, x, y, file, true);
				break;
			default:
				break;
			}

			ObjectsInScene.push_back(newObject);
		}

		if (m_pTheHero != nullptr)
		{
			for (auto monster : monsters)
				monster->m_HeroToChase = m_pTheHero;

			for (auto pickup : pickups)
				pickup->m_hero = m_pTheHero;
		}
	}
}

void Game::UnLoadObjects()
{
	for (int i = 0; i < ObjectsInScene.size(); i++)
		delete ObjectsInScene[i];

	ObjectsInScene.clear();

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
	//ImGui::DockSpaceOverViewport();
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

	else if (State == ENDGAME)
	{
		// Logic for the End Game Menu - a button press that either returns to Menu or to Game

		UpdateText("Press Q", 150, 250, m_pBigFont, { 255,255,255 });
		UpdateText("To Return to the Menu", 550, 270, m_pSmallFont, { 0,0,128, });

		UpdateText("Press Enter", 150, 350, m_pBigFont, { 255,255,255 });
		UpdateText("To Return to the Game", 550, 450, m_pSmallFont, { 0,255,0 });


		if (input->KeyIsPressed(KEY_RETURN))
		{
			UnLoadObjects();
			LoadObjects();
			State = Game::GAME;
		}
		if (input->KeyIsPressed(KEY_Q))
		{
			UnLoadObjects();
			LoadObjects();
			State = Game::SPLASH;
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

		for (auto object : ObjectsInScene)
		{
			object->Update();
			object->draw();
		}
		// Show our bitmaps
		//if (m_Pickup->isVisable)
		//	m_Pickup->draw();
		//if (!m_Pickup->isVisable)
		//{
		//	m_Goal2->draw();

		//}
		//m_Goal->draw();
		//m_pTheMonster->draw();
		//m_pTheHero->draw();	// The sequence of which the bitmaps are drawn is important
		//				// bitmaps drawn first are behind anything drawn after them!


		SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y };
		SDL_Rect spriteHeroRect = { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
		SDL_Rect spriteGoal2Rect = { m_Goal2->GetX(), m_Goal2->GetY(), m_Goal2->GetW(), m_Goal2->GetH() };


		// Implement chase function
		//m_pTheMonster->Chase();
		RenderObjectsWindow();

		if (showSelectionGui && m_SelectedObject != nullptr)
		{
			ImGui::Begin("Object Properties");

			bool checkboxValue = m_SelectedObject->applyGravity;
			if (ImGui::Checkbox("Apply Gravity", &checkboxValue))
			{
				m_SelectedObject->applyGravity = checkboxValue;
			}

			const char* type_names[(int)ObjectType::Object_Count] = { "Static", "Hero", "Monster", "Pickup" };
			int current_type = (int)m_SelectedObject->type;
			if (ImGui::Combo("combo", &current_type, type_names, IM_ARRAYSIZE(type_names)))
			{
				m_SelectedObject->type = (ObjectType)current_type;
			}

			if (ImGui::Button("Close Window", ImVec2(100, 30)))
			{
				showPickupImgui = false;
			};
			ImGui::End();
		}

		/*for (auto bitmap : ObjectsInScene)
		{
			// check if the mouse is over any bitmaps
			SDL_Rect objectRect = { bitmap->GetX(),bitmap->GetY(),bitmap->GetW(), bitmap->GetH() };
			if (SDL_PointInRect(&mousePoint, &objectRect))
			{
				// if so, make that bitmap selection
				m_SelectedObject = bitmap;
				break;
			}
		}*/

		AssetManager();
		RenderSceneHierarchy();
	
		// If mouse is down and nothing is selected
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && m_SelectedObject == nullptr)
		{
			for (auto bitmap : ObjectsInScene)
			{
				// check if the mouse is over any bitmaps
				SDL_Rect objectRect = { bitmap->GetX(),bitmap->GetY(),bitmap->GetW(), bitmap->GetH() };
				if (SDL_PointInRect(&mousePoint, &objectRect))
				{
					// if so, make that bitmap selection
					m_SelectedObject = bitmap;
					showSelectionGui = true;
					break;
				}
			}
		}

		// if bitmap is selected
		if (m_SelectedObject)
		{
			// move to mouse position
			m_SelectedObject->SetPosition(mousePoint.x - m_SelectedObject->GetW() / 2.0f, mousePoint.y - m_SelectedObject->GetH() / 2.0f);
		}

		// if mouse is released and bitmap is selected
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && m_SelectedObject != nullptr)
		{
			// clear bitmap selection
			m_SelectedObject = nullptr;
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && AssetMousDrag != nullptr)
		{
			cout << "Test" << endl;
			int x, y;
			SDL_GetMouseState(&x, &y);

			Bitmap* bitmap = new Bitmap(m_Renderer, AssetMousDrag->path, x, y, AssetMousDrag->Name, AssetMousDrag->transparent);
			ObjectsInScene.push_back(bitmap);

			//Sprite* s = new Sprite(pRenderer, AssetMousDrag->FileName, x, y, true, &io, AssetMousDrag->);
			//s->Transfrom.ParentSet(GameWindow::Instance().GetHirarcy());
			//sceneRoot.Children.push_back(&s->M_Transform);

			AssetMousDrag = nullptr;
		}
		
	}

	for (auto object : ObjectsInScene)
	{
		if (object->applyGravity)
			object->addoffset(0, 2);
	}

	SDL_Point mousePoint = { io->MousePos.x, io->MousePos.y };
	SDL_Rect spriteHeroRect = { m_pTheHero->GetX(), m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
	bool isMouseOverHero = SDL_PointInRect(&mousePoint, &spriteHeroRect);
	if (isMouseOverHero && io->MouseDown[0])
	{
		showGravityWindow = true;
	}

	int pickupCount = 0;
	int totalPickups = 0;

	for (auto object : ObjectsInScene)
	{
		if (object->type != ObjectType::Pickup || object->isEnd) continue;

		totalPickups++;

		if (!object->isVisable) pickupCount++;
	}

	bool hasCompleted = (pickupCount == totalPickups);

	if (hasCompleted)
	{
		SDL_Rect spriteHeroRect = { m_pTheHero->GetX(),m_pTheHero->GetY(),m_pTheHero->GetW(), m_pTheHero->GetH() };
		SDL_Rect spriteGoal2Rect = { m_Goal2->GetX(), m_Goal2->GetY(), m_Goal2->GetW(), m_Goal2->GetH() };
		SDL_Point heroPos{ m_pTheHero->GetX() + (m_pTheHero->GetW() / 2), m_pTheHero->GetY() + (m_pTheHero->GetH() / 2) };
		//SDL_Rect spritePickupRect = { GetX(), GetY(), GetW(), GetH() };

		if (SDL_PointInRect(&heroPos, &spriteGoal2Rect)&& State !=Game::ENDGAME)
		{
			State = Game::ENDGAME;
			cout << "Reached Goal" << endl;
		}
		
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

void Game::FindAssets()
{
	
	std::string path = "../Assets";
	for (const auto& entry : std::filesystem::directory_iterator(path)) //directory_iterator(path) //recursive_
	{
		if (entry.path().extension() == ".bmp" || entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
		{
			Bitmap* Asset = new Bitmap(m_Renderer, entry.path().string(), 0, 0, entry.path().string(), true);
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

void Game::SaveWorldData()
{
	json world;

	/*  {
    "type": 1,
    "x": 358,
    "y": 400,
    "file": "deadpool.bmp",
    "end": false
  },*/

	for (auto object : ObjectsInScene)
	{
		json obj;
		obj["type"] = (int)object->type;
		obj["x"] = object->m_x;
		obj["y"] = object->m_y;
		obj["file"] = object->path;
		obj["end"] = object->isEnd;
		world.push_back(obj);
	}

	std::ofstream stream{ "../assets/world.json" };
	stream << world;
	stream.close();
}

void Game::RenderObjectsWindow()
{
	//ImGui::NewFrame();
	ImGui::Begin("Objects");

	for (auto bitmap : ObjectsInScene)
	{
		if (ImGui::Button(bitmap->GetName().c_str()))
		{
			m_SelectedObject = bitmap;
		}
	}

	/*if (ImGui::Button("Select Hero"))
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
	}*/

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
	ImGui::Begin("Editor");
	ImGui::BeginChild("Content Window", ImVec2(), true);
	//ImGui::BeginTable("Content browser", 3);
	;
	for (int i = 0; i < content.size(); i++)
	{
		ImGui::PushID(i);

		ImGui::ImageButton((ImTextureID)content[i]->GetTextureRef(), { 100,100 });


		/////////////////////////////////For Draging
		if (ImGui::BeginDragDropSource())
		{
			AssetMousDrag = content[i];
			ImGui::Image((ImTextureID)content[i]->GetTextureRef(), { 100,100 });
			ImGui::EndDragDropSource();
		}
		/////////////////////////////////For Draging
		ImGui::PopID();
	}

	//ImGui::EndTabItem();

	ImGui::EndChild();
	ImGui::End();

	//ImGui::Begin("Asset Manager");

	//// Refresh button
	//if (ImGui::Button("Refresh"))
	//{
	//	// Reload the asset folder
	//}

	//// Create a collapsible tree structure for the file explorer
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
	//for (const auto& file : std::filesystem::directory_iterator("../assets"))
	//{
	//	if (file.is_directory())
	//	{
	//		bool node_open = ImGui::TreeNode(file.path().filename().string().c_str());
	//		if (node_open)
	//		{
	//			// Recursively display the subdirectory
	//			for (const auto& subfile : std::filesystem::directory_iterator(file.path()))
	//			{
	//				if (ImGui::Selectable(subfile.path().string().c_str()))
	//				{
	//					// Set the drag and drop payload when a file is selected
	//					if (ImGui::BeginDragDropSource())
	//					{
	//						ImGui::SetDragDropPayload("ASSET", subfile.path().string().c_str(), sizeof(char) * subfile.path().string().size(), ImGuiCond_Once);
	//						ImGui::EndDragDropSource();
	//					}
	//				}
	//			}
	//			ImGui::TreePop();
	//		}
	//	}
	//	else
	//	{
	//		if (ImGui::Selectable(file.path().string().c_str()))
	//		{
	//			// Set the drag and drop payload when a file is selected
	//			if (ImGui::BeginDragDropSource())
	//			{
	//				ImGui::SetDragDropPayload("ASSET", file.path().string().c_str(), sizeof(char) * file.path().string().size(), ImGuiCond_Once);
	//				ImGui::EndDragDropSource();
	//			}
	//		}
	//	}
	//}
	//ImGui::PopStyleVar();

	//// Drag and drop target
	//if (ImGui::BeginDragDropTarget())
	//{
	//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
	//	{
	//		// Get the dropped asset path
	//		std::string assetPath(reinterpret_cast<const char*>(payload->Data), payload->DataSize);

	//		// Load asset into the game
	//	}
	//	ImGui::EndDragDropTarget();
	//}

	//ImGui::End();
}


//void Game::AssetManager()
//{
//	ImGui::Begin("Asset Manager");
//	if (ImGui::Button("Refresh"))
//	{
//		// Reload the asset folder
//	}
//
//	for (const auto& file : std::filesystem::directory_iterator("../assets"))
//	{
//		if (ImGui::Selectable(file.path().string().c_str()))
//		{
//			// set the drag and drop payload when a file is selected
//			//ImGui::BeginDragDropSource();
//			//ImGui::SetDragDropPayload("ASSET", file.path().string().c_str(), sizeof(char) * file.path().string().size(), ImGuiCond_Once);
//		}
//	}
//
//	if (ImGui::BeginDragDropTarget())
//	{
//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET"))
//		{
//			// Get the dropped asset path
//			std::string assetPath(reinterpret_cast<const char*>(payload->Data), payload->DataSize);
//
//			// Load asset into the game
//		}
//		ImGui::EndDragDropTarget();
//	}
//	ImGui::End();
//}

Uint32 Game::ResetEvent = SDL_RegisterEvents(1);
Uint32 Game::PickupEvent = SDL_RegisterEvents(1);
