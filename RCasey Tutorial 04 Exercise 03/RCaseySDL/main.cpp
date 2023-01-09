#include "Game.h"


#undef main
int main(int argc, char* argv[])
{
	//CurrentState = GAME_SCREEN;

	//SDLm_Window::Instance().SetFullScreen(true); // << SDL INIT code here

	////imGUI Setup
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//SDL_DisplayMode DisplayMode;
	//SDL_GetCurrentDisplayMode(0, &DisplayMode);
	//ImGuiSDL::Initialize(m_Window::Instance().pRenderer, DisplayMode.w, DisplayMode.h);
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//ImGui::StyleColorsDark();

	//ImGui_ImplSDL2_InitForOpenGL(m_Window::Instance().pWindow, m_Window::Instance().gl_context);

	Game* game = new Game();	// create a new game object
	 // create an Input object

	if (game)
	{
		// decale and init variable used for colour background
		

		//check keys pressed to update variables
		while (!game->input->KeyIsPressed(KEY_ESCAPE))
		{
			
			game->Update(); // update the game


		}

		

		delete game;			//clean up
		game = nullptr;
	}

	return 0;		// exit nicely
}


