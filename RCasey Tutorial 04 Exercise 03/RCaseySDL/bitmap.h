#ifndef BITMAP_H
#define BITMAP_H

#include <string>
#include <iostream>
#include <SDL.h>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

enum class ObjectType
{
	Static,
	Hero,
	Monster,
	Pickup,
	Object_Count
};

/*{
	type: 0,
	x: 300,
	y: 400,
	file: "spritedata.bmp"
}*/

class Bitmap
{
private:
	SDL_Surface* m_pbitmapSurface;
	SDL_Texture* m_pbitmapTexture = nullptr;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_texture;
	
	
public:
	bool isVisable = true;
	bool isEnd = false;
	int m_x = 0, m_y = 0, m_w =0, m_h = 0;
	int spriteWidth = 69;
	float deltaTime = 0.0f;
	float m_vx = 0.0f, m_vy = 0.0f, m_ax = 0.0f, m_ay = 9.81f;
	std::string Name;
	std::string path;
	bool transparent;
	void SetParent(Bitmap* parent) { m_parent = parent; }
	Bitmap* GetParent() const { return m_parent; }
	void AddChild(Bitmap* child) { m_children.push_back(child); }
	std::vector<Bitmap*>& GetChildren() { return m_children; }

	ObjectType type{ ObjectType::Static };

	bool applyGravity{ false };

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
	SDL_Texture* GetTextureRef() { return m_pbitmapTexture; }

	bool IsDragged = false;
	bool ContainsPoint(int x, int y) const;

};
#endif
