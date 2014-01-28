#include "BrickBlock.h"
#include "Player.h"

BrickBlock::BrickBlock(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = STATIC;
	m_pSprite->LoadSpritesheet("brick_block", 96, 96, 5, 1);
	m_pSprite->AddAnimation("shatter", Vector2i(1,0), Vector2i(4,0), 4);
	m_collisionRect = CollisionRectangle(0,0,32,32);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_pSprite->m_bIsAnimated = false;
	m_bIsBouncing = false;
	m_bIsShattering = false;
	m_fBounceTimer = 0;
	m_staticPosition = m_position;
}


BrickBlock::~BrickBlock(void)
{
}


void BrickBlock::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_bIsShattering)
	{
		if(m_pSprite->m_sCurrentAnimation == "idle")
		{
			m_state = REMOVED;
		}
	}
	else if(m_bIsBouncing)
	{
		m_fBounceTimer += a_fDeltaTime;
		if(m_fBounceTimer < 0.08f)
		{
			Move(0, -90 * a_fDeltaTime);
		}
		else
		{
			Move(0, 90 * a_fDeltaTime);
			if(m_fBounceTimer > 0.16f)
			{
				m_position = m_staticPosition;
				m_bIsBouncing = false;
				m_fBounceTimer = false;
			}
		}
	}
}


void BrickBlock::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		if(m_position.y + m_collisionRect.y < a_pOther->GetPosition().y + a_pOther->m_collisionRect.y
			&& a_pOther->GetPosition().y + a_pOther->m_collisionRect.y - m_position.y + m_collisionRect.y < 34
			&& abs(a_pOther->GetPosition().x + a_pOther->m_collisionRect.x - m_position.x - m_collisionRect.x) < 24
			&& abs(a_pOther->GetPosition().x + a_pOther->m_collisionRect.x + a_pOther->m_collisionRect.width - m_position.x - m_collisionRect.x - m_collisionRect.width) < 24)
		{
			if(((Player*)(a_pOther))->GetIsLarge() && !m_bIsShattering)
			{
				m_pSprite->PlayAnimation("shatter");
				m_bIsShattering = true;
				m_type = EFFECT;
				Move(-32,-32);
				m_pSprite->m_bIsAnimated = true;
				Sounds->Play("smb_breakblock");
			}
			else
			{
				m_bIsBouncing = true;
				Sounds->Play("smb_bump");
			}
		}
	}
}