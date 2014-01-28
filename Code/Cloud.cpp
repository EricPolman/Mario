#include "Cloud.h"
#include "Player.h"

Cloud::Cloud(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = STATIC;
	m_pSprite->LoadSpritesheet("cloud", 94, 32, 1, 1);
	m_collisionRect = CollisionRectangle(0,0,94,32);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_pSprite->m_bIsAnimated = false;
	m_bIsMoving = false;
}


Cloud::~Cloud(void)
{
}


void Cloud::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_bIsMoving)
	{
		Move(70 * a_fDeltaTime, 0);
	}
}


void Cloud::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		m_bIsMoving = true;
	}
}