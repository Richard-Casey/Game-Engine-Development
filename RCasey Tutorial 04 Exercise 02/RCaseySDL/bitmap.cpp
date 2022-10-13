#include <string>
#include "bitmap.h"
#include "SDL.h"
#include "SDL_render.h"

using namespace std;



Bitmap::Bitmap(SDL_Renderer* renderer, string filename, int xpos, int ypos, bool useTransparency)
{
	
	//store the renderer for future configuring and drawing
	m_pRenderer = renderer;

	//create the bitmap surface
	m_pbitmapSurface = SDL_LoadBMP(filename.c_str());
	if (!m_pbitmapSurface)
	{
		//bitmap not loaded? output error
		printf("SURFACE for bitmap '%s' not loaded! \n", filename.c_str());
		printf("%s\n", SDL_GetError());
	}
	 
	else
	{
		//if we are to use the transparency, going to assume
		//the colour key is magenta (r=255, g=0, b=255)
		if (useTransparency)
		{
			Uint32 colourKey = SDL_MapRGB(m_pbitmapSurface->format, 255, 0, 255);
			SDL_SetColorKey(m_pbitmapSurface, SDL_TRUE, colourKey);
		}

		//create the texture
		m_pbitmapTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_pbitmapSurface);
		if (!m_pbitmapTexture)
		{
			//texture not loaded? Output an error
			printf("TEXTURE for bitmap '%s' not loaded! \n", filename.c_str());
			printf("%s\n", SDL_GetError());
		}
	}

	//store the position vals
	m_x = xpos;
	m_y = ypos;

	
}

void Bitmap::draw()
{
	//render the bitmap at the x/y coords
	if (m_pbitmapTexture)
	{
		SDL_Rect destRect = { m_x, m_y, m_pbitmapSurface->w, m_pbitmapSurface->h };
		SDL_RenderCopy(m_pRenderer, m_pbitmapTexture, NULL, &destRect);
	}
}

void Bitmap::addoffset(int x, int y)
{
	m_x += x;
	m_y += y;
}



Bitmap::~Bitmap()
{
	if (m_pbitmapTexture)
		SDL_DestroyTexture(m_pbitmapTexture);

	if (m_pbitmapSurface)
		SDL_FreeSurface(m_pbitmapSurface);
}

