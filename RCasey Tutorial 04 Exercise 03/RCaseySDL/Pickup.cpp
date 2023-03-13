#include "Creature.h"
#include "bitmap.h"
#include "Hero.h"
#include "Pickup.h"
#include "Game.h"

/// <summary>
/// Initializes a new instance of the <see cref="Pickup"/> class.
/// </summary>
/// <param name="hero">The hero.</param>
/// <param name="renderer">The renderer.</param>
/// <param name="filename">The filename.</param>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
/// <param name="name">The name.</param>
/// <param name="useTransparency">if set to <c>true</c> [use transparency].</param>
Pickup::Pickup(Hero* hero, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
    : Bitmap(renderer, filename, xpos, ypos, name, useTransparency)
{
    type = ObjectType::Pickup;
    m_hero = hero;
}

/// <summary>
/// Updates this instance.
/// </summary>
void Pickup::Update()
{
    if (isEnd) return;

    SDL_Point heroPos{ m_hero->GetX() + (m_hero->GetW() / 2), m_hero->GetY() + (m_hero->GetH() / 2) };
    SDL_Rect spritePickupRect = { GetX(), GetY(), GetW(), GetH() };

    if (SDL_PointInRect(&heroPos, &spritePickupRect))
    {
        isVisable = false;
        printf("Item collected\n");
        if (Game::PickupEvent != ((Uint32)-1)) {
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event));
            event.type = Game::PickupEvent;
            event.user.code = 2;
            event.user.data1 = 0;
            event.user.data2 = 0;
            SDL_PushEvent(&event);
        }
    }
}
