#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <iostream>
#include <SDL.h>

#include "nlohmann/json.hpp"

/// <summary>
/// JSON is used to save the x,y, style and filename of the Bitmaps
/// </summary>
using json = nlohmann::json;

/// <summary>
/// SDL Surface for Bitmaps
/// </summary>
struct SDL_Surface;

/// <summary>
/// SDL Texture to add to SDL Surface for Bitmaps
/// </summary>
struct SDL_Texture;

/// <summary>
/// Render call for SDL (Renders Texture and Surface for the bitmap)
/// </summary>
struct SDL_Renderer;

/// <summary>
/// Enum for type of bitmaps
/// </summary>
enum class ObjectType
{
	Static,
	Hero,
	Monster,
	Pickup,
	Object_Count
};

/// <summary>
/// Bitmap Class
/// </summary>
class Bitmap
{
private:
	/// <summary>
	/// Surface for bitmap
	/// </summary>
	SDL_Surface* m_pbitmapSurface;

	/// <summary>
	/// Texture for bitmap (initialised to nullptr)
	/// </summary>
	SDL_Texture* m_pbitmapTexture = nullptr;

	/// <summary>
	/// Renderer for bitmap
	/// </summary>
	SDL_Renderer* m_pRenderer;

	/// <summary>
	/// Texture for bitmap
	/// </summary>
	SDL_Texture* m_texture;
	
public:

	/// <summary>
	/// Boolean used for end goal (Bitmap is visable - collecting key results in false)
	/// </summary>
	bool isVisable = true;

	/// <summary>
	/// Boolean used for JSON save
	/// </summary>
	bool isEnd = false;

	/// <summary>
	/// Used withing bitmap.cpop to set the x, y, width and height of the bitmap
	/// </summary>
	int m_x = 0, m_y = 0, m_w =0, m_h = 0;

	/// <summary>
	/// Width of sprite
	/// </summary>
	int spriteWidth = 69;

	/// <summary>
	/// Initialise Delta Time to 0
	/// </summary>
	float deltaTime = 0.0f;

	/// <summary>
	/// Used in the draw function of Bitmap to calculate delta time
	/// </summary>
	float m_vx = 0.0f, m_vy = 0.0f, m_ax = 0.0f, m_ay = 9.81f;

	/// <summary>
	/// Used in bitmap constructor for name and to return Name for GetName() function
	/// </summary>
	std::string Name;

	/// <summary>
	/// Used in bitmap constructor for filepath
	/// </summary>
	std::string path;

	/// <summary>
	/// Used in bitmap constructor for usetransparency
	/// </summary>
	bool transparent;

	
	void SetParent(Bitmap* parent) { m_parent = parent; }
	Bitmap* GetParent() const { return m_parent; }
	void AddChild(Bitmap* child) { m_children.push_back(child); }
	std::vector<Bitmap*>& GetChildren() { return m_children; }

	ObjectType type{ ObjectType::Static };

	bool applyGravity{ false };

	std::vector<Bitmap*> children;

public:
	Bitmap(SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency = false);
	
	~Bitmap();
	void draw();
	void addoffset(int x, int y);

	virtual void Update() {}

	Bitmap* m_parent;
	std::vector<Bitmap*> m_children;

	int GetX() { return m_x; }
	int GetY() { return m_y; }
	int GetW();
	int GetH();
	int GetPositionX();
	int GetPositionY();
	void SetPosition(int x, int y);
	std::string GetName() const;
	std::string GetFilepath() const;
	SDL_Texture* GetTextureRef() { return m_pbitmapTexture; }

	bool IsDragged = false;
	bool ContainsPoint(int x, int y) const;

};
#endif
