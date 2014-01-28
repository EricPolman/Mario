#include "Star.h"
#include "Player.h"

Star::Star(Direction a_direction, float a_fX, float a_fY, Level* a_level)
	: Entity("star", a_fX, a_fY, a_level)
{
	m_type = PICKUP;
	m_bCollidesWithTiles = true;
	m_bUsesGravity = true;
	m_pSprite->LoadSpritesheet("star", 32, 32, 4, 1);
	m_pSprite->AddAnimation("flash", Vector2i(0,0), Vector2i(3,0), 3);
	m_pSprite->PlayAnimation("flash", true);
	m_direction = a_direction;

	m_StarState = STAR_SPAWNING;
	m_fSpawnDistanceTraveled = 0;
	m_zIndex = -1;
}


Star::~Star(void)
{
}


void Star::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);

	switch(m_StarState)
	{
	case STAR_SPAWNING:
		Move(0, -31 * a_fDeltaTime);
		m_fSpawnDistanceTraveled += 26 * a_fDeltaTime;
		if(m_fSpawnDistanceTraveled > 26)
		{
			m_StarState = STAR_MOVING;
			m_zIndex = 10;
		}
		break;
	case STAR_MOVING:
		m_position = m_position + m_velocity;
		m_velocity.y *= 0.85;

		if(m_direction == LEFT)
		{
			m_velocity.x -= 10 * a_fDeltaTime;
			if(m_velocity.x < -2) m_velocity.x = -2;
		}
		else
		{
			m_velocity.x += 10 * a_fDeltaTime;
			if(m_velocity.x > 2) m_velocity.x = 2;
		}
		break;
	}
	
}


void Star::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		m_state = REMOVED;
		((Player*)a_pOther)->Pickup(STAR);
	}
	else if(a_pOther->m_type == STATIC)
	{
		if(m_StarState == STAR_MOVING)
		{
			if(abs(a_info.depth.x) < abs(a_info.depth.y))
			{
				if(a_info.depth.x > 0 && m_velocity.x < 0 || a_info.depth.x < 0 && m_velocity.x > 0)
				{
					Move(a_info.depth.x + m_velocity.x, 0);
					m_velocity.x = 0;
				}
			}
			else
			{						
				if(a_info.depth.y > 0 && m_velocity.y < 0 || a_info.depth.y < 0 && m_velocity.y > 0)
				{
					Move(0,a_info.depth.y + m_velocity.y);
					m_velocity.y = 0;
					if(m_position.y + m_collisionRect.y < a_pOther->GetPosition().y + a_pOther->m_collisionRect.y)
					{
						Land();
					}
				}
			}
			a_pOther->Hit(this, a_info);
		}
	}
}