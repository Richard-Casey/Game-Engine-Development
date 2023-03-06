#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <iostream>
#include <SDL.h>


struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface;
	SDL_Texture* m_pbitmapTexture = nullptr;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_texture;
	

public:
	int m_x = 0, m_y = 0, m_w =0, m_h = 0;
	int spriteWidth = 69;
	float deltaTime = 0.0f;
	float m_vx = 0.0f, m_vy = 0.0f, m_ax = 0.0f, m_ay = 9.81f;
	std::string Name;
	

public:
	Bitmap(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency = false);
	
	~Bitmap();
	void draw();
	void addoffset(int x, int y);

	int GetX() { return m_x; }
	int GetY() { return m_y; }
	int GetW();
	int GetH();
	int GetPositionX();
	int GetPositionY();
	void SetPosition(int x, int y);
	std::string GetName() const;

	bool IsDragged = false;
	bool ContainsPoint(int x, int y) const;

};

#endif
