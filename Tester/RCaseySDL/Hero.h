#pragma once
#include "Creature.h"
#include "Hero.h"
#include "bitmap.h"


class Hero : public Creature
{
public:
	Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency);
	~Hero();
	void Update()
	{

	}

    // Getters and setters
    float getX() const { return m_x; }
    void setX(float x) { m_x = x; }

    float getY() const { return m_y; }
    void setY(float y) { m_y = y; }

    float getSpeed() const { return m_speed; }
    void setSpeed(float speed) { m_speed = speed; }

    float m_x;
    float m_y;

private:
 
    int m_speed;

};
