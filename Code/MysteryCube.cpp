#include "MysteryCube.h"
#include "Player.h"
#include "Mushroom.h"
#include "CoinEffect.h"
#include "Star.h"

MysteryCube::MysteryCube(MysteryReward a_reward, float a_fX, float a_fY, Level* a_level)
	: Entity("blank", a_fX, a_fY, a_level)
{
	m_reward = a_reward;

	m_type = EntityType::STATIC;
	m_pSprite->LoadSpritesheet("mystery_cube", 32, 32, 3, 1);
	m_pSprite->AddAnimation("shine", Vector2i(0,0), Vector2i(1,0),1);
	m_pSprite->AddAnimation("dull", Vector2i(2,0), Vector2i(2,0));
	m_pSprite->PlayAnimation("shine", true);
	m_collisionRect = CollisionRectangle(0,0, m_pSprite->GetWidth(), m_pSprite->GetHeight());
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	
	m_fBounceTimer = 0;
	m_bIsBouncing = false;

	m_staticPosition = m_position;
}


MysteryCube::~MysteryCube(void)
{
}


void MysteryCube::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);
	if(m_bIsBouncing)
	{
		m_fBounceTimer += a_fDeltaTime;
		if(m_fBounceTimer < 0.08f)
		{
			Move(0, -64 * a_fDeltaTime);
		}
		else
		{
			Move(0, 64 * a_fDeltaTime);
			if(m_fBounceTimer > 0.16f)
			{
				m_position = m_staticPosition;
				m_bIsBouncing = false;
				m_fBounceTimer = false;
			}
		}
	}
}


void MysteryCube::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(m_pSprite->m_sCurrentAnimation != "dull")
	{
		if(a_pOther->m_type == PLAYER)
		{
			Player* player = (Player*)a_pOther;

			if(m_position.y + m_collisionRect.y < a_pOther->GetPosition().y + a_pOther->m_collisionRect.y
				&& a_pOther->GetPosition().y + a_pOther->m_collisionRect.y - m_position.y + m_collisionRect.y < 34)
			{
				m_pSprite->PlayAnimation("dull",true);
				m_bIsBouncing = true;
				Sounds->Play("smb_bump");
				switch(m_reward)
				{					
				default:
				case COINS:
					Stats::AddCoins(10);
					new CoinEffect(m_position.x, m_position.y - 128);
					Sounds->Play("smb_coin");
					break;
				case RED_MUSHROOM:
					if(player->GetIsLarge())
					{
						Stats::AddCoins(10);
						new CoinEffect(m_position.x, m_position.y - 128);
						Sounds->Play("smb_coin");
					}
					else
					{
						new Mushroom(RIGHT, m_position.x, m_position.y - 6);
						Sounds->Play("smb_powerup_appears");
					}
					break;
				case STAR:
					if(player->GetIsLarge())
					{
						new Star(RIGHT,m_position.x, m_position.y - 6);
					}
					else
					{
						new Mushroom(RIGHT, m_position.x, m_position.y - 6);
						Sounds->Play("smb_powerup_appears");
					}
					break;
				}
			}
		}
	}
}