#include "Coin.h"
#include "Player.h"

Coin::Coin(float a_fX, float a_fY, Level* a_level)
	: Entity("coin", a_fX, a_fY, a_level)
{
	m_pSprite->LoadSpritesheet("coin", 20, 28, 3, 1);
	m_pSprite->AddAnimation("spin", Vector2i(0,0), Vector2i(2,0), 1);
	m_pSprite->PlayAnimation("spin",true);
	m_type = PICKUP;
	m_zIndex = -1;
}


Coin::~Coin(void)
{
}


void Coin::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		m_state = REMOVED;
		Stats::CURRENT_SCORE += 1;
		Sounds->Play("smb_coin");
	}
}