#include "CoinEffect.h"
#include "Player.h"

CoinEffect::CoinEffect(float a_fX, float a_fY, Level* a_level)
	: Entity("coin_effect", a_fX, a_fY, a_level)
{
	m_type = EFFECT;
	m_pSprite->LoadSpritesheet("coin_effect", 32, 128, 7, 1);
	m_pSprite->AddAnimation("spin", Vector2i(0,0), Vector2i(6,0),3.5f);
	m_pSprite->PlayAnimation("spin",false);
	m_zIndex = 10;
}


CoinEffect::~CoinEffect(void)
{
}


void CoinEffect::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_pSprite->m_sCurrentAnimation == "idle")
	{
		m_state = REMOVED;
	}
}