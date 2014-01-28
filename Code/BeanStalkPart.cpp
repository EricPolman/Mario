#include "BeanStalkPart.h"
#include "Player.h"

BeanStalkPart::BeanStalkPart(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = TRIGGER;
	m_pSprite->LoadSpritesheet("beanstalk_part", 32, 32, 1, 1);
	m_collisionRect = CollisionRectangle(0,0,32,32);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_bIsBouncing = false;
	m_bIsShattering = false;
	m_fBounceTimer = 0;
	m_staticPosition = m_position;
	m_zIndex = -5;
}


BeanStalkPart::~BeanStalkPart(void)
{
}


void BeanStalkPart::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
}


void BeanStalkPart::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		if(abs(a_pOther->GetPosition().x + a_pOther->m_collisionRect.x - m_position.x) < 16)
			((Player*)a_pOther)->Climb();
	}
}