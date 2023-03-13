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

// Set the display color for rendering
void Game::SetDisplayColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// Clean up
	// Don't forget - we destroy in the reverse order that they were created
	if (m_Renderer)
	{
		// Set the render color
		int result = SDL_SetRenderDrawColor(
			m_Renderer,		// Our target renderer
			r,				// R
			g,				// G
			b,				// B
			a				// Alpha
		);
	}
}

// Update text on screen
void Game::UpdateText(std::string msg, int x, int y, TTF_Font* font, SDL_Color colour)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, msg.c_str(), colour);
	if (!surface)
	{
		// Handle error when surface is not loaded
		printf("SURFACE for font not loaded! \n");
		printf("%s\n", SDL_GetError());
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	if (!texture)
	{
		// Handle error when texture is not loaded
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

// Check events for the game
void Game::CheckEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != NULL)
	{
		// Update input
		input->Update(e);

		// Reset game event
		if (e.type == Game::ResetEvent && e.user.code == 1)
		{
			UnLoadObjects();
			monsters.clear();
			pickups.clear();
			LoadObjects();
		}

		// Pickup event
		if (e.type == Game::PickupEvent && e.user.code == 2)
		{
			// m_Goal->isVisable = false;
			m_Goal2->isVisable = true;
		}
	}
}

// Generate a random number
double Game::randomNumber()
{
	static bool need_random = true;
	if (need_random)
	{
		// Seed the random number generator
		srand(static_cast<unsigned int>(time(NULL)));
		need_random = false;
	}

	// Generate the random number
	return static_cast<double>(rand()) / RAND_MAX;
}



// Constructor for the Game class
Game::Game()
{
	// Initialize member variables
	m_running = true;
	m_Window = nullptr;
	m_Renderer = nullptr;

	// Initialize SDL and TTF
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	// Create the window
	m_Window = SDL_CreateWindow(
		"My First Window",					// title
		SDL_WINDOWPOS_CENTERED,				// initial x position
		SDL_WINDOWPOS_CENTERED,				// initial y position
		ScreenWidth,						// width, in pixels
		ScreenHeight,						// height in pixels
		SDL_WINDOW_RESIZABLE);				// window behaviour flags (ignore for now) - can be OR'd together using ||

	// Create the OpenGL context
	SDL_GLContext SDL_GLContext = SDL_GL_CreateContext(m_Window);

	// Handle error if window creation fails
	if (!m_Window)
	{
		printf("WINDOW initialization failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}

	// Create the renderer
	m_Renderer = SDL_CreateRenderer(
		m_Window,			// link the renderer to our newly created win
		-1,					// index rendering driver (ignore for now)
		0					// renderer behaviour flags (ignore for now)
	);

	// Handle error if renderer creation fails
	if (!m_Renderer)
	{
		printf("RENDERER initialization failed %s\n", SDL_GetError());
		printf("Press any key to continue\n");

		return;
	}

	// imGUI Setup - this needs to be after the renderer is created
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	ImGuiSDL::Initialize(m_Renderer, DisplayMode.w, DisplayMode.h);

	io = &ImGui::GetIO();
	(void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GLContext);

	// Load objects, fonts, and assets
	LoadObjects();
	m_pSmallFont = TTF_OpenFont("../assets/FatPixels.ttf", 25);
	m_pBigFont = TTF_OpenFont("../assets/FatPixels.ttf", 75);
	m_pInfoFont = TTF_OpenFont("../assets/Condensed.ttf", 25);
	FindAssets();

	// Register the ResetEvent
	Game::ResetEvent = SDL_RegisterEvents(1);

	//ImGui::Begin("Profiler");

	//static FrameMap* Snapshot;
	//static vector<float>* FrameTimes;
	////take snapshot of all current frame data.
	//if (ImGui::Button("take snapshot"))
	//{
	//	//mem copy?
	//	Snapshot = &(ProfilerSystem::Instance().GetFrameDate());
	//	FrameTimes = &(ProfilerSystem::Instance().GetFrameTimes());
	//}
	//ImGui::SameLine();
	//static bool LiveFlameGraph = true;
	//ImGui::Checkbox("Live Flame Graph", &LiveFlameGraph);
	//if (LiveFlameGraph)
	//{
	//	selectedFrame = -1;
	//}
	//static int range[2] = { 0, 100 };

	//if (FrameTimes && FrameTimes->size() > 100)
	//{
	//	ImGui::SliderInt2("Frame Range", range, 0, FrameTimes->size());
	//	if (range[0] >= range[1])
	//		range[0] = range[1] - 1;
	//	/*ImGui::SliderInt("Frame Range", &range[0], 0, FrameTimes->size());
	//	if (range[0] <10)
	//		range[0] =10;*/
	//	vector<float> subData(FrameTimes->cbegin() + range[0], FrameTimes->cbegin() + range[1]);
	//	//vector<float> subData(FrameTimes->cbegin() + range[0]-10, FrameTimes->cbegin() + range[0]);

	//	int tempHistSelection = ImGui::MyPlotHistogram("Frame data", subData.data(), subData.size());
	//	if (tempHistSelection != -1)
	//	{
	//		LiveFlameGraph = false;
	//		selectedFrame = tempHistSelection;
	//	}

	//	//cout << t << endl;
	//	//get mouse in hostogram
	//	ImRect rect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };
	//	if (rect.Contains(io.MousePos))
	//	{
	//		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	//		{
	//			cout << selectedFrame << endl;
	//		}
	//	}

	//}


	////float GraphWindowWidth = ImGui::CalcItemWidth();
	//FrameMap& previousFrame = ProfilerSystem::Instance().GetLastFrameData();
	//if (!LiveFlameGraph && selectedFrame != -1)
	//{
	//	previousFrame.clear();
	//	for (auto const [SampleName, samples] : *Snapshot)
	//	{
	//		previousFrame[SampleName].push_back(samples[range[0] + selectedFrame]);
	//	}
	//}
	//else
	//{
	//	LiveFlameGraph = false;
	//}
	////ImGui::PlotHistogram("FrameTimes")
	//ImGui::LabelText("Frame Date Count", std::to_string(previousFrame.size()).c_str());
	////ImGui::LabelText("Graph Window Width", std::to_string(GraphWindowWidth).c_str());
	////ImGui::RenderFrame({ 10,20 }, { 100,50 }, ImGui::GetColorU32(ImGuiCol_FrameBg), true, GImGui->Style.FrameRounding);
	//ImDrawList* drawlist = ImGui::GetCurrentWindow()->DrawList;
	//ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();//pos of screen top left
	//ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	//if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	//if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	//ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	//drawlist->PushClipRect(canvas_p0, canvas_p1, true);


	//uint64_t totalframeTime = 0;
	//vector<uint64_t> SampleTimes;
	//vector<float> SampleWidths;
	//vector<string> SampleNames;
	//for (auto const& [SampleName, samples] : previousFrame)
	//{
	//	totalframeTime += samples[0]->frameTime + 1;
	//	SampleTimes.push_back(samples[0]->frameTime + 1);
	//	SampleNames.push_back(SampleName);
	//}
	//float MinBlockWith = canvas_sz.x / totalframeTime;// GraphWindowWidth / totalframeTime;
	//for (int i = 0; i < SampleTimes.size(); i++)
	//{
	//	SampleWidths.push_back(SampleTimes[i] * MinBlockWith);
	//}

	////ImGui::LabelText("Total window width", std::to_string(GraphWindowWidth).c_str());
	//ImGui::LabelText("Total Frame Time", std::to_string(totalframeTime).c_str());
	//ImGui::LabelText("Window Width / total frame Time", std::to_string(MinBlockWith).c_str());
	//float TotalBlockWidthSoFar = 0;

	//int sampleCount = previousFrame.size();
	////ImGui::SliderInt("Iterations", &sampleCount, 1, 10);
	////sampleCount = 2;
	////MinBlockWith = GraphWindowWidth / sampleCount;

	//const ImU32 col_outline_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
	//const ImU32 col_base = ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;

	//float f = 5.0f + 5.0f;

	//for (int i = 0; i < sampleCount; i++)
	//{
	//	float ThisBlockWidth = SampleWidths[i];
	//	//if (ThisBlockWidth == 0)
	//		//ThisBlockWidth = MinBlockWith;

	//	const ImVec2 minPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar, canvas_p0.y + 100);
	//	//float a = canvas_sz.x - (canvas_p1.x - (canvas_p1.x*i))
	//	const ImVec2 maxPos = ImVec2(canvas_p0.x + TotalBlockWidthSoFar + ThisBlockWidth, canvas_p0.y + 200);
	//	drawlist->AddRectFilled(
	//		minPos,
	//		maxPos,
	//		col_base,
	//		GImGui->Style.FrameRounding);

	//	drawlist->AddRect(minPos, maxPos, col_outline_base);

	//	ImGui::RenderText(ImVec2(minPos.x + 10, minPos.y + 10), SampleNames[i].c_str());
	//	ImGui::RenderText(ImVec2(minPos.x + 10, minPos.y + 20), std::to_string(SampleTimes[i] - 1).c_str());


	//	TotalBlockWidthSoFar += ThisBlockWidth;
	//}
	//drawlist->PopClipRect();

	//ImGui::End();

}

// This function is used to convert a Bitmap object into a JSON object.
void to_json(json& j, const Bitmap& p) {
	j = json{
	{"type", (int)p.type}, // The type of the Bitmap object
	{"x", p.m_x}, // The x-coordinate of the Bitmap object
	{"y", p.m_y}, // The y-coordinate of the Bitmap object
	{"file", p.path} // The file path of the Bitmap object
	};
}

// This function is used to convert a JSON object into a Bitmap object.
void from_json(const json& j, Bitmap& p) {
	j.at("type").get_to(p.type); // The type of the Bitmap object
	j.at("x").get_to(p.m_x); // The x-coordinate of the Bitmap object
	j.at("y").get_to(p.m_y); // The y-coordinate of the Bitmap object
	j.at("file").get_to(p.path); // The file path of the Bitmap object
}

// This function is used to load objects from a JSON file.
void Game::LoadObjects()
{
	

	// Open the JSON file containing the game object data
	std::ifstream f("../assets/world.json");
	if (f.good())
	{
		// Parse the JSON data
		json data = json::parse(f);

		// Loop through the JSON object data and create new game objects based on the object type
		for (auto obj : data)
		{
			Bitmap* newObject = nullptr;

			ObjectType type = obj.at("type"); // Get the object type
			std::string file = obj.at("file"); // Get the file path for the object
			int x = obj.at("x"); // Get the x-coordinate of the object
			int y = obj.at("y"); // Get the y-coordinate of the object

			// Create a new game object based on its type
			switch (type)
			{
			case ObjectType::Hero:
				newObject = new Hero(m_Renderer, file, x, y, "Hero", true); // Create a new hero object
				m_pTheHero = static_cast<Hero*>(newObject); // Set the hero object as the main hero in the game
				break;
			case ObjectType::Monster:
				newObject = new Monster(nullptr, m_Renderer, file, x, y, "Monster", true); // Create a new monster object
				monsters.push_back(static_cast<Monster*>(newObject)); // Add the monster object to the monsters vector
				break;
			case ObjectType::Pickup:
			{
				bool endGoal = obj.at("end"); // Check if the pickup object is an end goal
				newObject = new Pickup(nullptr, m_Renderer, file, x, y, "Pickup", true); // Create a new pickup object
				if (endGoal)
				{
					m_Goal2 = static_cast<Pickup*>(newObject); // Set the pickup object as the second goal if it is an end goal
					m_Goal2->isEnd = true;
				}
				pickups.push_back(static_cast<Pickup*>(newObject)); // Add the pickup object to the pickups vector
				break;
			}
			case ObjectType::Static:
				newObject = new Bitmap(m_Renderer, file, x, y, file, true); // Create a new static object
				break;
			default:
				break;
			}

			ObjectsInScene.push_back(newObject); // Add the new game object to the objects in the scene vector
		}

		// Assign the hero object to each monster and pickup object in the scene
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
	// delete all objects in the scene
	for (int i = 0; i < ObjectsInScene.size(); i++)
		delete ObjectsInScene[i];

	// clear the scene vector and nullify all object pointers
	ObjectsInScene.clear();
	m_pTheHero = nullptr;
	m_pTheMonster = nullptr;
	m_Pickup = nullptr;
	m_Goal = nullptr;
	m_Goal2 = nullptr;
}

Game::~Game()
{
	// Clean up all resources allocated by the game
	// Destroy input
	delete input;
	input = nullptr;

	// Destroy monster object if exists
	if (m_pTheMonster)
		delete m_pTheMonster;

	// Destroy hero object if exists
	if (m_pTheHero)
		delete m_pTheHero;

	// Destroy pickup object if exists
	if (m_Pickup)
		delete m_Pickup;

	// Destroy goal object if exists
	if (m_Goal)
		delete m_Goal;

	// Close fonts used in the game
	TTF_CloseFont(m_pBigFont);
	TTF_CloseFont(m_pSmallFont);

	// Destroy the renderer
	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);
	}

	// Destroy the window
	if (m_Window)
	{
		SDL_DestroyWindow(m_Window);
	}
}

void Game::Update(void)
{
	CheckEvents();  // Check for any events (e.g. key presses) and update the input manager accordingly
	SDL_RenderClear(m_Renderer);  // Clear the renderer
	ImGui::NewFrame();  // Start a new GUI frame
	ImGui_ImplSDL2_NewFrame(m_Window);  // Start a new SDL2 frame


	for (const auto x : monsters) {

		
		x->Update();
	
	}

	// SPLASH state
	if (State == Game::SPLASH)
	{
		// Show a menu with a button that starts the game if ENTER is pressed
		UpdateText("Press Enter", 210, 250, m_pBigFont, { 255,255,255 });
		UpdateText("To Start", 275, 320, m_pBigFont, { 255,255,255 });
		UpdateText("Simple proof of concept game in which you retrieve the key to allow you to return to the start and complete the level", 10, 650, m_pInfoFont, { 200, 240, 236 });
		UpdateText("all while avoiding the Grim Reaper.", 350, 675, m_pInfoFont, { 223,62,62 });

		// If ENTER is pressed, switch to GAME state
		if (input->KeyIsPressed(KEY_RETURN))
		{
			State = Game::GAME;
		}
	}

	// ENDGAME state
	else if (State == ENDGAME)
	{
		// Show a menu with buttons to either return to the SPLASH state or start a new game
		UpdateText("Press Q", 150, 250, m_pBigFont, { 255,255,255 });
		UpdateText("To Return to the Menu", 550, 270, m_pSmallFont, { 0,0,128, });
		UpdateText("Press Enter", 150, 350, m_pBigFont, { 255,255,255 });
		UpdateText("To Return to the Game", 550, 450, m_pSmallFont, { 0,255,0 });

		// If ENTER is pressed, start a new game
		if (input->KeyIsPressed(KEY_RETURN))
		{
			UnLoadObjects();
			LoadObjects();
			State = Game::GAME;
		}

		// If Q is pressed, return to the SPLASH state
		if (input->KeyIsPressed(KEY_Q))
		{
			UnLoadObjects();
			LoadObjects();
			State = Game::SPLASH;
		}
	}

	// GAME state
	else if (State == Game::GAME)
	{
		// Change the display colour based on user input
		if (input->KeyIsPressed(KEY_R))
		{
			if (++r > 255) r = 0;
		}
		if (input->KeyIsPressed(KEY_G))
		{
			if (++g > 255) g = 0;
		}
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
			std::cout << "Test" <<std::endl;
			int x, y;
			SDL_GetMouseState(&x, &y);

			Bitmap* bitmap = new Bitmap(m_Renderer, AssetMousDrag->path, x, y, AssetMousDrag->Name, AssetMousDrag->transparent);
			ObjectsInScene.push_back(bitmap);

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

		if (SDL_PointInRect(&heroPos, &spriteGoal2Rect)&& State !=Game::ENDGAME)
		{
			State = Game::ENDGAME;
			std::cout << "Reached Goal" << std::endl;
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
			//ObjectsInScene[i].
			ImGui::TreePop();
		}
	}

	ImGui::End();
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
	ImGui::Begin("Objects");

	for (auto bitmap : ObjectsInScene)
	{
		if (ImGui::Button(bitmap->GetName().c_str()))
		{
			m_SelectedObject = bitmap;
		}
		
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

	ImGui::Begin("Save World State");
		if (ImGui::Button("Save World"))
		{
			SaveWorldData();
		}
		ImGui::End();
	
	
}

void Game::AssetManager()
{
	ImGui::Begin("Editor");
	ImGui::BeginChild("Content Window", ImVec2(), true);
	;
	for (int i = 0; i < content.size(); i++)
	{
		ImGui::PushID(i);

		ImGui::ImageButton((ImTextureID)content[i]->GetTextureRef(), { 100,100 });


		 // For Dragging
		if (ImGui::BeginDragDropSource())
		{
			AssetMousDrag = content[i];
			ImGui::Image((ImTextureID)content[i]->GetTextureRef(), { 100,100 });
			ImGui::EndDragDropSource();
		}
		//For Dragging
		ImGui::PopID();
	}

	ImGui::EndChild();
	ImGui::End();
}

Uint32 Game::ResetEvent = SDL_RegisterEvents(1);
Uint32 Game::PickupEvent = SDL_RegisterEvents(1);