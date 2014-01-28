#include "Flagpole.h"
#include "Player.h"
#include "SoundManager.h"

Flagpole::Flagpole(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = TRIGGER;
	m_pSprite->LoadSpritesheet("flagpole", 16, 304, 1, 1);
	m_collisionRect = CollisionRectangle(0,0, m_pSprite->GetWidth(), m_pSprite->GetHeight());
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_pSprite->m_bIsAnimated = false;
	m_bFlagDown = false;
	m_zIndex = -1;
	m_pFlag = new Flag(m_position.x + 10, m_position.y + 16);
	m_bPullDownInitiated = false;
}


Flagpole::~Flagpole(void)
{
}


void Flagpole::Update(float a_fDeltaTime)
{
	if(m_bPullDownInitiated && !m_bFlagDown)
	{
		m_pFlag->Move(0, 375 * a_fDeltaTime);
	}
}


void Flagpole::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		if(!m_bFlagDown && !m_bPullDownInitiated)
		{
			((Player*)(a_pOther))->PullDownFlag(this);
			m_bPullDownInitiated = true;
			Sounds->Play("smb_flagpole");
		}
	}
}