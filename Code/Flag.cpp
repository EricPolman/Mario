#include "Flag.h"
#include "Player.h"

Flag::Flag(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = TRIGGER;
	m_pSprite->LoadSpritesheet("flag", 32, 32, 4, 1);
	m_pSprite->AddAnimation("wave", Vector2i(0,0), Vector2i(3,0),2);
	m_pSprite->PlayAnimation("wave", true);

	m_collisionRect = CollisionRectangle(0,0, m_pSprite->GetWidth(), m_pSprite->GetHeight());
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_zIndex = 0;
}


Flag::~Flag(void)
{
}


void Flag::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_position.y > 352)
		m_position.y = 352;
}


void Flag::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		
	}
}