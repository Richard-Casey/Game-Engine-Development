#pragma once
#include "bitmap.h"

using namespace std;

class Creature : public Bitmap
{
public:

	Creature(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency = false);
	~Creature();
	void Update()
	{
		
	}
};

//class Hero  : public Creature
//{
//public:
//	Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency = false);
//	~Hero();
//	void Update();
//private:
//
//};

//Hero::Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, bool useTransparency) : Creature(renderer, filename, xpos, ypos, useTransparency)
//{
//	this->Update(); // Specifies that the update from Hero needs to be used instead of the Update from Creature
//}
//
//Hero::~Hero()
//{
//}

