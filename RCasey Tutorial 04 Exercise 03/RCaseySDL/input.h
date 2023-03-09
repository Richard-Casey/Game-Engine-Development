#pragma once
#include <SDL.h>

enum KEYS_PRESSED_LIST
{
	KEY_ESCAPE, KEY_R, KEY_G, KEY_B, KEY_W, KEY_S, KEY_A, KEY_D, KEY_U, KEY_J, KEY_H, KEY_K, KEY_RETURN, KEY_Q, SIZE_OF_KEYS_PRESSED_ENUM
};

class Input
{
public:
	SDL_Event m_event;
	void Update(SDL_Event m_event);
	Input();
	~Input();	
	bool m_keysPressed[SIZE_OF_KEYS_PRESSED_ENUM] = {};
	bool KeyIsPressed(KEYS_PRESSED_LIST key);
};

