#include "Goomba.h"


Goomba::Goomba(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY)
{
	m_pSprite->LoadSpritesheet("goomba", 32, 32, 3, 1);
	m_pSprite->AddAnimation("walk", Vector2i(0,0), Vector2i(1,0), 1.0f);
	m_pSprite->AddAnimation("flat", Vector2i(2,0), Vector2i(2,0), 0.9f);
	m_pSprite->PlayAnimation("walk", true);
	m_pSprite->m_bIsAnimated = true;
	m_collisionRect = CollisionRectangle(0,4,32,28);
	m_type = ENEMY;
	m_bCollidesWithTiles = true;
	m_bUsesGravity = true;
	m_zIndex = 2;
	m_direction = LEFT;
}


Goomba::~Goomba(void)
{
}


void Goomba::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(!m_bIsSmashed)
	{
		m_position = m_position + m_velocity;
		m_velocity.y *= 0.85;

		if(m_bIsOnFloor)
		{
			if(m_direction == LEFT)
			{
				m_velocity.x -= 10 * a_fDeltaTime;
				if(m_velocity.x < -1) m_velocity.x = -1;
			}
			else
			{
				m_velocity.x += 10 * a_fDeltaTime;
				if(m_velocity.x > 1) m_velocity.x = 1;
			}
		}
		else
		{
			m_velocity.x = 0;
		}
	}
	else
	{
		if(m_pSprite->m_sCurrentAnimation == "idle")
		{
			m_state = REMOVED;
			Stats::AddCoins(1);
		}
	}
}


void Goomba::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	switch(a_pOther->m_type)
	{
	case PICKUP:

		break;
	case PLAYER:
		if(m_position.y + m_collisionRect.y + m_collisionRect.height <
				a_pOther->GetPosition().y + a_pOther->m_collisionRect.y + a_pOther->m_collisionRect.height + 6)
		{
			a_pOther->Hurt();
		}
	case ENEMY:
		if(!m_bIsSmashed)
		{
			Move(a_info.depth.x + m_velocity.x, 0);
			m_velocity.x = 0;
			Bounce();
		}
		break;
	default:
	case STATIC:
		if(abs(a_info.depth.x) < abs(a_info.depth.y))
		{
			if(a_info.depth.x > 0 && m_velocity.x < 0 || a_info.depth.x < 0 && m_velocity.x > 0)
			{
				Move(a_info.depth.x + m_velocity.x, 0);
				m_velocity.x *= -1;
				Bounce();
			}
		}
		else
		{						
			if(a_info.depth.y > 0 && m_velocity.y < 0 || a_info.depth.y < 0 && m_velocity.y > 0)
			{
				Move(0,a_info.depth.y + m_velocity.y);
				m_velocity.y = 0;
			}
		}
		break;
	}
}


void Goomba::Smash()
{
	if(!m_bIsSmashed)
	{
		Hurt();
		Sounds->Play("smw_smash");
	}
}


void Goomba::Hurt()
{
	m_bIsSmashed = true;
	m_pSprite->PlayAnimation("flat", false);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_velocity = Vector2();
}