#pragma once
#include "bitmap.h"

class Creature : public Bitmap
{
public:
	Creature(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency = false);
	~Creature();
};
