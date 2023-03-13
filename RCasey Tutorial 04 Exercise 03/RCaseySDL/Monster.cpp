#include "Monster.h"
#include "Creature.h"
#include "Hero.h"

/// <summary>
/// Initializes a new instance of the <see cref="Monster"/> class.
/// </summary>
/// <param name="HeroToChase">The hero to chase.</param>
/// <param name="renderer">The renderer.</param>
/// <param name="filename">The filename.</param>
/// <param name="xpos">The xpos.</param>
/// <param name="ypos">The ypos.</param>
/// <param name="name">The name.</param>
/// <param name="useTransparency">if set to <c>true</c> [use transparency].</param>
Monster::Monster(Hero* HeroToChase, SDL_Renderer* renderer, std::string filename, int xpos, int ypos, std::string name, bool useTransparency)
	: Creature(renderer, filename, xpos, ypos, name, useTransparency)
{
	type = ObjectType::Monster;
	m_HeroToChase = HeroToChase;
}

/// <summary>
/// Finalizes an instance of the <see cref="Monster"/> class.
/// </summary>
Monster::~Monster()
{
}

/// <summary>
/// Chases this instance.
/// </summary>
void Monster::Chase()
{
	if (m_HeroToChase == nullptr)
		return; // do nothing if m_HeroToChase is null

	int diffx = m_HeroToChase->GetX() - m_x;
	int diffy = m_HeroToChase->GetY() - m_y;

	if (diffx > 0)
		diffx = 1;
	else if (diffx < 0)
		diffx = -1;

	if (diffy > 0)
		diffy = 1;
	else if (diffy < 0)
		diffy = -1;

	m_x += diffx;
	m_y += diffy;

	if (m_x == m_HeroToChase->GetX() && m_y == m_HeroToChase->GetY())
	{
		if (Game::ResetEvent != ((Uint32)-1))
		{
			SDL_Event event;
			SDL_memset(&event, 0, sizeof(event));
			event.type = Game::ResetEvent;
			event.user.code = 1;
			event.user.data1 = 0;
			event.user.data2 = 0;
			SDL_PushEvent(&event);
		}
	}
}

