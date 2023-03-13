#include "TextureManager.h"



/// <summary>
/// Initializes a new instance of the <see cref="TextureManager"/> class.
/// </summary>
TextureManager::TextureManager()
{
}


/// <summary>
/// Finalizes an instance of the <see cref="TextureManager"/> class.
/// </summary>
TextureManager::~TextureManager()
{
	for (auto value : m_textureMap)
	{
		delete value.second;
		value.second = nullptr;
	}
}

/// <summary>
/// Unloads the specified file name.
/// </summary>
/// <param name="fileName">Name of the file.</param>
void TextureManager::Unload(const std::string fileName)
{
	auto searchResult = m_textureMap.find(fileName);
	if(searchResult != m_textureMap.end())
	{
		delete m_textureMap[fileName];
		m_textureMap[fileName] = nullptr;
		m_textureMap.erase(searchResult);
	}
}

/// <summary>
/// Loads the specified file name.
/// </summary>
/// <param name="fileName">Name of the file.</param>
/// <param name="useTransparancy">if set to <c>true</c> [use transparancy].</param>
/// <param name="PRenderer">The p renderer.</param>
/// <returns></returns>
SDL_Texture* TextureManager::Load(const std::string fileName, bool useTransparancy, SDL_Renderer* PRenderer)
{
	SDL_Texture* m_pbitmapTexture = nullptr;
	
	//check map for file name existance
	auto searchresult = m_textureMap.find(fileName);
	if (searchresult != m_textureMap.end())
	{
		m_pbitmapTexture = m_textureMap[fileName];
	}
	else // not found, load, save and return
	{

		//if file does not exist in map
		SDL_Surface* pTempSurface = SDL_LoadBMP(fileName.c_str());
		if (!pTempSurface)
		{
			printf("SURFACE for bitmap '%s' not loaded\n", fileName.c_str());
			printf("%s\n", SDL_GetError());
		}
		else
		{
			if (useTransparancy)
			{
				Uint32 colourKey = SDL_MapRGB(pTempSurface->format, 255, 0, 255);
				SDL_SetColorKey(pTempSurface, SDL_TRUE, colourKey);
			}
			m_pbitmapTexture = SDL_CreateTextureFromSurface(PRenderer, pTempSurface);
			if (!m_pbitmapTexture)
			{
				printf("TEXTURE for bitmap '%s' not loaded!\n", fileName.c_str());
				printf("%s\n", SDL_GetError());
			}

			m_textureMap[fileName] = m_pbitmapTexture;
		}
	}
	return m_pbitmapTexture;
}


