#include "KoopaTroopa.h"


KoopaTroopa::KoopaTroopa(float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY)
{
	m_pSprite->LoadSpritesheet("turtle", 32, 48, 4, 2);
	m_pSprite->AddAnimation("3_walk", Vector2i(0,0), Vector2i(1,0), 1.0f);
	m_pSprite->AddAnimation("1_walk", Vector2i(2,0), Vector2i(3,0), 1.0f);
	m_pSprite->AddAnimation("smashed_idle", Vector2i(0,1), Vector2i(0,1), 0.05f);
	m_pSprite->AddAnimation("smashed_spin", Vector2i(0,1), Vector2i(3,1), 2);
	m_pSprite->PlayAnimation("3_walk", true);
	m_pSprite->m_bIsAnimated = true;
	m_collisionRect = CollisionRectangle(0,4,32,44);
	m_type = ENEMY;
	m_bCollidesWithTiles = true;
	m_bUsesGravity = true;
	m_zIndex = 2;
	m_direction = LEFT;
	m_fInvulnerableTimer = 0;
	m_bIsVulnerable = true;
	m_bDashing = false;
}


KoopaTroopa::~KoopaTroopa(void)
{
}


void KoopaTroopa::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(!m_bIsVulnerable)
	{
		m_fInvulnerableTimer += a_fDeltaTime;
		if(m_fInvulnerableTimer > 0.1f)
		{
			m_fInvulnerableTimer = 0;
			m_bIsVulnerable = true;
		}
	}

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
		if(m_bDashing)
		{
			m_position = m_position + m_velocity;
			m_velocity.y *= 0.85;
			if(m_direction == LEFT)
			{
				m_velocity.x -= 80 * a_fDeltaTime;
				if(m_velocity.x < -4) m_velocity.x = -4;
			}
			else
			{
				m_velocity.x += 80 * a_fDeltaTime;
				if(m_velocity.x > 4) m_velocity.x = 4;
			}
		}
		else{
			if(m_pSprite->m_sCurrentAnimation == "idle")
			{
				m_bIsSmashed = false;
				m_bIsVulnerable = true;
				if(m_direction == LEFT)
				{
					m_pSprite->PlayAnimation("3_walk", true);
				}
				else
				{
					m_pSprite->PlayAnimation("1_walk", true);
				}
				m_collisionRect = CollisionRectangle(0,0,32,48);
			}
		}
	}
}


void KoopaTroopa::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	switch(a_pOther->m_type)
	{
	case PICKUP:

		break;
	case PLAYER:
		if(!m_bDashing && m_bIsSmashed && m_bIsVulnerable)
		{
			m_bDashing = true;
			m_pSprite->PlayAnimation("smashed_spin", true);
			Sounds->Play("smb_kick");
			SetDirection(
				a_pOther->GetPosition().x + a_pOther->m_collisionRect.x < m_position.x
				? RIGHT : LEFT);
		}
		else if(m_bDashing && m_bIsVulnerable)
		{
			if(m_position.x - a_pOther->GetPosition().x < 0) //Player is to the right.
			{
				if(m_velocity.x > 0.1)
					a_pOther->Hurt();
			}
			else if(m_position.x - a_pOther->GetPosition().x > 0) //Player is to the right.
			{
				if(m_velocity.x < -0.1)
					a_pOther->Hurt();
			}
		}
		break;
	case ENEMY:
		if(!m_bIsSmashed && !m_bDashing)
		{
			Move(a_info.depth.x + m_velocity.x, 0);
			m_velocity.x = 0;
			Bounce();
		}
		else if(m_bIsSmashed && m_bDashing)
		{
			a_pOther->Hurt();
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


void KoopaTroopa::Smash()
{
	if(!m_bIsSmashed && m_bIsVulnerable)
	{
		m_bIsSmashed = true;
		m_pSprite->PlayAnimation("smashed_idle", false);
		m_velocity = Vector2();
		m_bIsVulnerable = false;
		m_collisionRect = CollisionRectangle(0,24,32,24);
		Sounds->Play("smw_smash");
	}
	else if(m_bIsSmashed && m_bIsVulnerable && !m_bDashing)
	{
		//m_bDashing = true;
		//m_pSprite->PlayAnimation("smashed_spin", true);
	}
}


void KoopaTroopa::Bounce()
{
	SwitchDirection();
	if(m_direction == LEFT)
	{
		if(m_bDashing)
			m_pSprite->PlayAnimation("smashed_spin", true);
		else
			m_pSprite->PlayAnimation("3_walk", true);
	}
	else
	{
		if(m_bDashing)
			m_pSprite->PlayAnimation("smashed_spin", true);
		else
			m_pSprite->PlayAnimation("1_walk", true);
	}
}