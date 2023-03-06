#pragma once
#include <string>
#include <unordered_map>
#include "SDL.h"

class ResourceManager
{
private:
	ResourceManager();
	ResourceManager(ResourceManager const&);
	ResourceManager operator = (ResourceManager const&);

	std::unordered_map<std::string, SDL_Surface*> m_Surfaces;

public:
	static ResourceManager& GetInstance();
	SDL_Surface* GetSurface(std::string filepath);
	static ResourceManager* s_instance;
};

