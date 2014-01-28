#include "BeanStalkCube.h"
#include "Player.h"
#include "TriggerWarp.h"
#include "TriggerManager.h"

BeanStalkCube::BeanStalkCube(std::string a_destination, float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_type = STATIC;
	m_pSprite->LoadSpritesheet("beanstalk_cube", 32, 32, 2, 1);
	m_pSprite->AddAnimation("brick", Vector2i(0,0), Vector2i(0,0),10);
	m_pSprite->AddAnimation("used", Vector2i(1,0), Vector2i(1,0),10);
	m_collisionRect = CollisionRectangle(0,0,32,32);
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_pSprite->m_bIsAnimated = false;
	m_bIsUsed = false;
	m_bBeanStalkGrown = false;
	m_destination = a_destination;
	m_fMoveTimer = 0.5f;
}


BeanStalkCube::~BeanStalkCube(void)
{
}


void BeanStalkCube::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_bIsUsed && !m_bBeanStalkGrown)
	{
		if(m_beanstalkParts.size() < 10)
		{
			m_fMoveTimer += a_fDeltaTime;

			if(m_fMoveTimer > 0.5f)
			{
				m_fMoveTimer -= 0.5f;
				m_beanstalkParts.push_back(new BeanStalkPart(m_position.x, m_position.y));
			}
		}
		else
		{
			m_bBeanStalkGrown = true;
		}

		for(std::vector<BeanStalkPart*>::iterator iter = m_beanstalkParts.begin();
			iter != m_beanstalkParts.end(); iter++)
		{
			(*iter)->Move(0,-64 * a_fDeltaTime);
		}
	}
}


void BeanStalkCube::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(a_pOther->m_type == PLAYER)
	{
		if(m_position.y + m_collisionRect.y < a_pOther->GetPosition().y + a_pOther->m_collisionRect.y
			&& a_pOther->GetPosition().y + a_pOther->m_collisionRect.y - m_position.y + m_collisionRect.y < 34
			&& abs(a_pOther->GetPosition().x + a_pOther->m_collisionRect.x - m_position.x - m_collisionRect.x) < 24
			&& abs(a_pOther->GetPosition().x + a_pOther->m_collisionRect.x + a_pOther->m_collisionRect.width - m_position.x - m_collisionRect.x - m_collisionRect.width) < 24)
		{
			if(!m_bIsUsed)
			{
				m_bIsUsed = true;
				Sounds->Play("smb_vine");
				m_pSprite->PlayAnimation("used", true);
				Triggers->Add(new TriggerWarp(m_position.x, 0, m_destination, 128, 320));
			}
		}
	}
}