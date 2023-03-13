#include <string>
#include "bitmap.h"
#include "SDL.h"
#include "SDL_render.h"
#include "ResourceManager.h"

/// <summary>
/// Gets the w.
/// </summary>
/// <returns></returns>
int Bitmap::GetW()
{
	return this->m_pbitmapSurface->w;
}

/// <summary>
/// Gets the h.
/// </summary>
/// <returns></returns>
int Bitmap::GetH()
{
	return this->m_pbitmapSurface->h;
}

/// <summary>
/// Gets the position x.
/// </summary>
/// <returns></returns>
int Bitmap::GetPositionX()
{
	return m_x;
}

/// <summary>
/// Gets the position y.
/// </summary>
/// <returns></returns>
int Bitmap::GetPositionY()
{
	return m_y;
}

/// <summary>
/// Sets the position.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
void Bitmap::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}



/// <summary>
/// Initializes a new instance of the <see cref="Bitmap" /> class.
/// </summary>
/// <param name="renderer">The renderer.</param>
/// <param name="filename">The filename.</param>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
/// <param name="name">The name.</param>
/// <param name="useTransparency">if set to <c>true</c> [use transparency].</param>
Bitmap::Bitmap(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)// : deltaTime(0.0f) // initialize deltaTime to 0
{
	Name = name;
	path = filename;
	transparent = useTransparency;
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
		SDL_Surface* retrieveSurface1 = ResourceManager::GetInstance().GetSurface(filename);
		m_pbitmapTexture = SDL_CreateTextureFromSurface(m_pRenderer, retrieveSurface1);
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

/// <summary>
/// Draws this instance.
/// </summary>
void Bitmap::draw()
{
	if (!isVisable) return;
	// update the object's position based on its velocity and acceleration
	m_vx += m_ax * deltaTime; // deltaTime is the time elapsed since the last frame
	m_vy += m_ay * deltaTime;
	m_x += m_vx * deltaTime;
	m_y += m_vy * deltaTime;

	//render the bitmap at the updated position
	if (m_pbitmapTexture)
	{
		SDL_Rect destRect = { m_x, m_y, m_pbitmapSurface->w, spriteWidth, };
		SDL_RenderCopy(m_pRenderer, m_pbitmapTexture, NULL, &destRect);
	}
}

/// <summary>
/// Addoffsets the specified x.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
void Bitmap::addoffset(int x, int y)
{
	m_x += x;
	m_y += y;
}

/// <summary>
/// Finalizes an instance of the <see cref="Bitmap"/> class.
/// </summary>
Bitmap::~Bitmap()
{
	if (m_pbitmapTexture)
		SDL_DestroyTexture(m_pbitmapTexture);

	if (m_pbitmapSurface)
		SDL_FreeSurface(m_pbitmapSurface);
}

// function definition for GetName()
/// <summary>
/// Gets the name.
/// </summary>
/// <returns></returns>
std::string Bitmap::GetName() const
{
	return Name;
}

std::string Bitmap::GetFilepath() const
{
	return path;
}
