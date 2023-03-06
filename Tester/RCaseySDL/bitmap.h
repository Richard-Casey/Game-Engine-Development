#ifndef BITMAP_H
#define BITMAP_H
#include <string>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface;
	SDL_Texture* m_pbitmapTexture = NULL;
	SDL_Renderer* m_pRenderer;
	//std::string m_name; // added member variable to store name of bitmap

public:
	int m_x, m_y;
	int spriteWidth = 69;
	//int spriteHeight = 81
	const float GRAVITY = 500.0f;
	float deltaTime;
	float m_vx = 0.0f; // velocity in x direction
	float m_vy = 0.0f; // velocity in y direction
	float m_ax = 0.0f; // acceleration in x direction
	float m_ay = 9.81f; // default value for m_ay
	int GetPositionX();
	int GetPositionY();
	void SetPosition(int x, int y);
	std::string GetName() const; // added function declaration

public:
	Bitmap(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency = false );
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

	std::string Name;

};


#endif
