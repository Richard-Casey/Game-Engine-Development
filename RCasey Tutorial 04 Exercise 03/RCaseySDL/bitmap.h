#pragma once
#include <string>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface;
	SDL_Texture* m_pbitmapTexture;
	SDL_Renderer* m_pRenderer;


public:
	int m_x, m_y;


public:
	Bitmap(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency = false);
	~Bitmap();
	void draw();
	void addoffset(int x, int y);
	
	int GetX()
	{
		return m_x;
	}
	int  GetY()
	{
		return m_y;
	}
	int GetW();
	int  GetH();
	

};