#include "ResourceManager.h"
#include <iostream>

/// <summary>
/// Prevents a default instance of the <see cref="ResourceManager"/> class from being created.
/// </summary>
ResourceManager::ResourceManager()
{

}

/// <summary>
/// Initializes a new instance of the <see cref="ResourceManager"/> class.
/// </summary>
/// <param name="">The .</param>
ResourceManager::ResourceManager(ResourceManager const&)
{

}

/// <summary>
/// Operator=s the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns></returns>
ResourceManager ResourceManager::operator = (ResourceManager const& rhs)
{
	//TODO although we do NOT need this for a singleton
	return *this;
}

// Instantiate exactly one instance of our class
/// <summary>
/// Gets the instance.
/// </summary>
/// <returns></returns>
ResourceManager& ResourceManager::GetInstance()
{
	return *s_instance;
}

/// <summary>
/// Gets the surface.
/// </summary>
/// <param name="filepath">The filepath.</param>
/// <returns></returns>
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