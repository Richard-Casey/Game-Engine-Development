#pragma once
#include "Creature.h"
#include "Hero.h"
#include "bitmap.h"


class Hero : public Creature
{
public:
	Hero(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency);
	~Hero();
    /// <summary>
    /// Updates this instance.
    /// </summary>
    void Update()
	{

	}
    
    // Getters and setters
/// <summary>
/// Gets the x.
/// </summary>
/// <returns></returns>
    float getX() const { return m_x; }
    /// <summary>
    /// Sets the x.
    /// </summary>
    /// <param name="x">The x.</param>
    void setX(float x) { m_x = x; }

    /// <summary>
    /// Gets the y.
    /// </summary>
    /// <returns></returns>
    float getY() const { return m_y; }
    /// <summary>
    /// Sets the y.
    /// </summary>
    /// <param name="y">The y.</param>
    void setY(float y) { m_y = y; }

    /// <summary>
    /// Gets the speed.
    /// </summary>
    /// <returns></returns>
    float getSpeed() const { return m_speed; }
    /// <summary>
    /// Sets the speed.
    /// </summary>
    /// <param name="speed">The speed.</param>
    void setSpeed(float speed) { m_speed = speed; }

    /// <summary>
    /// The m x
    /// </summary>
    float m_x;
    /// <summary>
    /// The m y
    /// </summary>
    float m_y;

private:
 
    /// <summary>
    /// The m speed
    /// </summary>
    int m_speed;

};
