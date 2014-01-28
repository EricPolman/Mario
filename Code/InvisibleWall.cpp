#include "InvisibleWall.h"
#include "Player.h"

InvisibleWall::InvisibleWall(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = STATIC;
	m_pSprite->LoadSpritesheet("invisible_wall", 32, 32, 1, 1);
	m_collisionRect = CollisionRectangle(0,0,32,32);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_pSprite->m_bIsAnimated = false;
}


InvisibleWall::~InvisibleWall(void)
{
}