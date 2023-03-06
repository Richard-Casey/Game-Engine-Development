#pragma once
#include <map>
#include <SDL.h>
#include <string>

class TextureManager
{
public:
	std::map<std::string, SDL_Texture*> m_textureMap;
	TextureManager();
	~TextureManager();

	SDL_Texture* Load(const std::string fileName, bool useTransparancy, SDL_Renderer* PRenderer);
	void Unload(const std::string fileName);
};

