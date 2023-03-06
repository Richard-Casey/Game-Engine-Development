#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() 
{

}

ResourceManager::ResourceManager(ResourceManager const&) 
{

}

ResourceManager ResourceManager::operator = (ResourceManager const& rhs)
{
	//TODO although we do NOT need this for a singleton
	return *this;
}

// Instantiate exactly one instance of our class
ResourceManager& ResourceManager::GetInstance()
{
	return *s_instance;
}

SDL_Surface* ResourceManager::GetSurface(std::string filepath)
{
	// We will look through our m_durfaces which is an unordered map and see if a file exists
	// And if the file does exist then return the associeated surface.
	auto search = m_Surfaces.find(filepath);
	if (search == m_Surfaces.end())
	{
		std::cout << "Image allocated once" + filepath << std::endl;
		SDL_Surface* surface = SDL_LoadBMP(filepath.c_str());
		m_Surfaces.insert(std::make_pair(filepath, surface));
	}

	return m_Surfaces[filepath];

}
ResourceManager* ResourceManager::s_instance = new ResourceManager();