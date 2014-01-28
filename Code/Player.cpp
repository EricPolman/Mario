#include "Player.h"
#include "World.h"
#include "Smashable.h"
#include "Flagpole.h"

#include "BeanStalkCube.h"

bool Player::g_bStarActive;

std::ostringstream sstream;
Player::Player(std::string a_tilesheetName, std::string a_sLevel, float a_fX, float a_fY)
	: Entity(a_tilesheetName, a_fX, a_fY, World::GetLevel(a_sLevel))
{
	m_pSprite->LoadSpritesheet(a_tilesheetName, 96, 96, 6, 4);
	
	m_pSprite->AddAnimation("1_walk", Vector2i(0,0), Vector2i(2,0), 2.5f);
	m_pSprite->AddAnimation("3_walk", Vector2i(0,1), Vector2i(2,1), 2.5f);
	
	m_pSprite->AddAnimation("1_idle", Vector2i(0,0), Vector2i(0,0), 2.5f);
	m_pSprite->AddAnimation("3_idle", Vector2i(0,1), Vector2i(0,1), 2.5f);
	
	m_pSprite->AddAnimation("1_jump", Vector2i(3,0), Vector2i(3,0), 2.5f);
	m_pSprite->AddAnimation("3_jump", Vector2i(3,1), Vector2i(3,1), 2.5f);

	m_pSprite->AddAnimation("3_braking", Vector2i(4,0), Vector2i(4,0), 2.5f);
	m_pSprite->AddAnimation("1_braking", Vector2i(4,1), Vector2i(4,1), 2.5f);
	
	m_pSprite->AddAnimation("1_walk_large", Vector2i(0,2), Vector2i(2,2), 2.5f);
	m_pSprite->AddAnimation("3_walk_large", Vector2i(0,3), Vector2i(2,3), 2.5f);
	
	m_pSprite->AddAnimation("1_idle_large", Vector2i(0,2), Vector2i(0,2), 2.5f);
	m_pSprite->AddAnimation("3_idle_large", Vector2i(0,3), Vector2i(0,3), 2.5f);
	
	m_pSprite->AddAnimation("1_jump_large", Vector2i(3,2), Vector2i(3,2), 2.5f);
	m_pSprite->AddAnimation("3_jump_large", Vector2i(3,3), Vector2i(3,3), 2.5f);

	m_pSprite->AddAnimation("3_braking_large", Vector2i(4,2), Vector2i(4,2), 2.5f);
	m_pSprite->AddAnimation("1_braking_large", Vector2i(4,3), Vector2i(4,3), 2.5f);
	
	m_pSprite->AddAnimation("frontfacing", Vector2i(5,0), Vector2i(5,0), 2.5f);
	m_pSprite->AddAnimation("frontfacing_large", Vector2i(5,2), Vector2i(5,2), 2.5f);

	m_pSprite->AddAnimation("pull_down", Vector2i(5,1), Vector2i(5,1), 2.5f);
	m_pSprite->AddAnimation("pull_down_large", Vector2i(5,3), Vector2i(5,3), 2.5f);
	
	m_pSprite->AddAnimation("enter_castle", Vector2i(0,4), Vector2i(3,4), 2.5f);
	m_pSprite->AddAnimation("enter_castle_large", Vector2i(0,5), Vector2i(3,5), 2.5f);
	
	m_pSprite->AddAnimation("1_changesize", Vector2i(0,6), Vector2i(5,6), 3);
	m_pSprite->AddAnimation("3_changesize", Vector2i(0,7), Vector2i(5,7), 3);
	
	m_pSprite->AddAnimation("1_climb", Vector2i(4,5), Vector2i(5,5), 2);
	m_pSprite->AddAnimation("3_climb", Vector2i(4,8), Vector2i(5,8), 2);
	m_pSprite->AddAnimation("1_climb_large", Vector2i(0,8), Vector2i(1,8), 2);
	m_pSprite->AddAnimation("3_climb_large", Vector2i(2,8), Vector2i(3,8), 2);
	
	m_pSprite->AddAnimation("die_idle", Vector2i(4,4), Vector2i(4,4), 2.5f);

	m_type = PLAYER;
}


void Player::Spawn()
{
	m_state = ALIVE;
	m_fRespawnTimer = 0;
	m_direction = RIGHT;
	m_bDirectionChanged = false;
	m_bIsLarge = false;
	m_bClearedLevel = false;
	sstream << m_direction;
	sstream.str("");
	m_fInvulnerableTimer = 0;
	m_bIsVulnerable = true;
	g_bStarActive = false;
	
	m_velocity = Vector2();

	m_collisionRect.x = 34;
	m_collisionRect.y = 64;
	m_collisionRect.height = 32;
	m_collisionRect.width = 24;

	m_playerState = PLAYER_IDLE;
	m_bHasFireFlower = true;
	m_bDied = false;

	m_fClimbTimer = 0;
	m_flagPole = 0;
	m_fJumpBoostTimer = 0;
	m_bCollidesWithTiles = true;
	m_bUsesGravity = true;
	m_fStarActiveTimer = 0;
}

Player::~Player()
{
	Entity::~Entity();
}

void Player::Update(float a_fDeltaTime)
{
	Entity::Update(a_fDeltaTime);

	m_position = m_position + m_velocity;
	m_velocity.y *= 0.85;
	m_velocity.x *= 0.9;
	if(abs(m_velocity.x) < 0.1)
	{
		m_velocity.x = 0;
	}
	
	if(g_bStarActive)
	{
		m_fStarActiveTimer += a_fDeltaTime;
		if(m_fStarActiveTimer > 7)
		{
			m_fStarActiveTimer = 0;
			g_bStarActive = false;
			Sounds->SetMusic("bgm",true);
		}
	}

	std::string isLarge = m_bIsLarge ? "_large" : "";

	//m_velocity = Vector2();
	Vector2 moveDirection;
	if(m_state == EntityState::ALIVE)
	{
		if(m_position.y > 550)
		{
			Die();
		}
		if(!m_bIsVulnerable)
		{
			if((int)(m_fInvulnerableTimer * 20) % 2 == 0)
			{
				m_bIsVisible = false;
			}
			else
			{
				m_bIsVisible = true;
			}
			m_fInvulnerableTimer += a_fDeltaTime;
			if(m_fInvulnerableTimer > 1)
			{
				m_fInvulnerableTimer = 0;
				m_bIsVulnerable = true;
				m_bIsVisible = true;
			}
		}

		switch(m_playerState)
		{
		case PLAYER_IDLE:
		case PLAYER_WALKING:
			if(Input->IsKeyPressed(SDLK_w) || Input->IsKeyPressed(SDLK_UP))
			{
				if(m_bIsOnFloor)
				{
					Sounds->Play("smb_jump"+isLarge);
					m_velocity.y -= 18;
					m_position.y -= 1;
					m_bIsOnFloor = false;
					if(m_direction == LEFT)
					{
						m_pSprite->PlayAnimation("3_jump"+isLarge, true);
					}
					else
					{
						m_pSprite->PlayAnimation("1_jump"+isLarge, true);
					}
				}
				//moveDirection.y -= 1;
			}
			if(m_bIsOnFloor && m_fJumpBoostTimer == 0)
			{
				if(Input->IsKeyDown(SDLK_a) || Input->IsKeyDown(SDLK_LEFT))
				{
					if(m_pSprite->m_sCurrentAnimation != "3_walk"+isLarge)
					{
						if(m_velocity.x > 0)
						{
							if(m_pSprite->m_sCurrentAnimation != "3_braking"+isLarge)
							{
								m_pSprite->PlayAnimation("3_braking"+isLarge, true);
							}
						}
						else
						{
							ChangeDirection(LEFT);
						}
					}
					moveDirection.x -= 1;
				}
				if(Input->IsKeyDown(SDLK_d) || Input->IsKeyDown(SDLK_RIGHT))
				{
					if(m_pSprite->m_sCurrentAnimation != "1_walk"+isLarge)
					{
						if(m_velocity.x < 0)
						{
							if(m_pSprite->m_sCurrentAnimation != "1_braking"+isLarge)
							{
								m_pSprite->PlayAnimation("1_braking"+isLarge, true);
							}
						}
						else
						{
							ChangeDirection(RIGHT);
						}
					}
					moveDirection.x += 1;
				}
			

				if(moveDirection.x != 0)
				{
					//moveDirection.Normalize();
				}
				else if(m_velocity.x == 0 && moveDirection.y == 0)
				{
					sstream << m_direction;
					if(m_pSprite->m_sCurrentAnimation != sstream.str()+"_changesize")
						m_pSprite->PlayAnimation(sstream.str()+"_idle"+isLarge, true);
					sstream.str("");
					m_bDirectionChanged = false;
				}
				
				m_velocity.x += moveDirection.x * a_fDeltaTime * (Input->IsKeyDown(SDLK_q) ? 30 : 20);
				m_velocity.x = m_velocity.x > (Input->IsKeyDown(SDLK_q) ? 5 : 3) ? (Input->IsKeyDown(SDLK_q) ? 5 : 3) : m_velocity.x;
				m_velocity.x = m_velocity.x < -(Input->IsKeyDown(SDLK_q) ? 5 : 3) ? -(Input->IsKeyDown(SDLK_q) ? 5 : 3) : m_velocity.x;
			}
			else
			{
				if(Input->IsKeyDown(SDLK_a) || Input->IsKeyDown(SDLK_LEFT))
				{
					if(m_velocity.y < 0)
						m_velocity.x -= 8.0f * a_fDeltaTime;
				}
				if(Input->IsKeyDown(SDLK_d) || Input->IsKeyDown(SDLK_RIGHT))
				{
					if(m_velocity.y < 0)
						m_velocity.x += 8.0f * a_fDeltaTime;
				}
				m_fJumpBoostTimer += a_fDeltaTime;
				if((Input->IsKeyDown(SDLK_w) || Input->IsKeyPressed(SDLK_UP)) && m_velocity.y < 0)
				{
					if(m_fJumpBoostTimer < 0.4)
					{
						m_velocity.y -= 70.0f * a_fDeltaTime;
					}
					//m_velocity.y *= 1/.85f;
					
				}

				m_velocity.x *= 1/.9f;
				if(m_velocity.x > (Input->IsKeyDown(SDLK_q) ? 5 : 3))
				{
					m_velocity.x = (Input->IsKeyDown(SDLK_q) ? 5 : 3);
				}
				else if(m_velocity.x < -(Input->IsKeyDown(SDLK_q) ? 5 : 3))
				{
					m_velocity.x = -(Input->IsKeyDown(SDLK_q) ? 5 : 3);
				}
			}
			//m_velocity = moveDirection * a_fDeltaTime * 400;
			
			if(Input->IsKeyPressed(SDLK_SPACE))
			{
				
			}
			else if(Input->IsKeyPressed(SDLK_e))
			{
				
			}
			break;
		case PLAYER_LOWERING_FLAG:
			if(!m_bIsOnFloor)
			{
				Stats::CURRENT_SCORE += 2;
				//m_velocity.y += a_fDeltaTime * 10;
			}
			else
			{
				((Flagpole*)m_flagPole)->m_bFlagDown = true;
				m_playerState = PLAYER_WALK_TO_CASTLE;
				m_pSprite->PlayAnimation("1_walk" + isLarge, true);
			}
			break;
		case PLAYER_WALK_TO_CASTLE:
			m_velocity.x = 100 * a_fDeltaTime;
			break;
		case PLAYER_ENTERING_CASTLE:
			m_velocity.x = 0;
			if(m_pSprite->m_sCurrentAnimation == "idle")
			{
				m_bClearedLevel = true;
			}
			break;
		case PLAYER_IN_TUBE:
			m_fTubeDistanceTraveled += 32 * a_fDeltaTime;
			switch(m_tubeDirection)
			{
			case DOWN:
				Move(0, 32 * a_fDeltaTime);
				break;
			case RIGHT:
				Move(32 * a_fDeltaTime,0);
				break;
			case UP:
				Move(0, -32 * a_fDeltaTime);
				break;
			case LEFT:
				Move(-32 * a_fDeltaTime,0);
				break;
			}
			if(m_fTubeDistanceTraveled > 56)
			{
				//m_position = m_tubeTargetPosition;
				m_playerState = PLAYER_IDLE;
				World::ChangeLevel(m_tubeTargetLevel, this, m_tubeTargetPosition);
			}
			break;
		case PLAYER_CLIMBING:
			m_fClimbTimer += a_fDeltaTime;
			if(m_fClimbTimer > 0.1f)
			{
				m_pSprite->PlayAnimation("3_idle"+isLarge);
				m_playerState = PLAYER_IDLE;
				m_bUsesGravity = true;
			}
			else
			{
				Move(0, -64 * a_fDeltaTime);
			}
			break;
		}
	}
	else if(m_state == DEAD)
	{
		m_pSprite->PlayAnimation("die_idle", true);
		m_fRespawnTimer += a_fDeltaTime;
		Move(0,100 * a_fDeltaTime);
		if(m_fRespawnTimer > 3)
		{
			World::ResetCameraPositions();
			Stats::CURRENT_SCORE = 0;
			m_bDied = true;
			Sounds->Resume();
		}

		if(m_velocity.y > 0)
			m_velocity.y *= 0.85f;
	}
}


void Player::ChangeDirection(Direction a_direction)
{
	std::string isLarge = m_bIsLarge ? "_large" : "";
	m_direction = a_direction;
	sstream << m_direction;
	m_pSprite->PlayAnimation(sstream.str()+"_walk"+isLarge, true);
	sstream.str("");
	m_bDirectionChanged = true;
}


void Player::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{
	if(m_state != DEAD)
	{
		switch(a_pOther->m_type)
		{
		case PICKUP:
			break;
		case ENEMY:
			if(m_position.y + m_collisionRect.y + m_collisionRect.height < 
				a_pOther->GetPosition().y + a_pOther->m_collisionRect.y + a_pOther->m_collisionRect.height - 6)
			{
				m_velocity.y = -12;
				m_bIsOnFloor = false;
				dynamic_cast<Smashable*>(a_pOther)->Smash();
			}
			else
			{
				if(!g_bStarActive)
				{
					if(!dynamic_cast<Smashable*>(a_pOther)->m_bIsSmashed)
						Hurt();
				}
				else
				{
					dynamic_cast<Smashable*>(a_pOther)->Smash();
				}
			}
		case DESTROYABLE:
		case STATIC:
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
			break;
		case ENTITY:
		case TRIGGER:
			a_pOther->Hit(this, a_info);
			break;
		default:
			break;
		}
	}
}


void Player::Die()
{
	m_pSprite->PlayAnimation("die_idle", true);
	Sounds->Pause();
	Sounds->Play("smb_mariodie");
	m_state = DEAD;
	
	m_velocity.y = -24;
	m_velocity.x = 0;
	m_bCollidesWithTiles = false;
}


void Player::Land(){
	std::string isLarge = m_bIsLarge ? "_large" : "";
	if(!m_bIsOnFloor){
		m_fJumpBoostTimer = 0;
		m_bIsOnFloor = true;
		if(m_direction == LEFT && m_pSprite->m_sCurrentAnimation == "3_jump"+isLarge && m_fJumpBoostTimer != 0)
		{
			m_pSprite->PlayAnimation("3_idle", true);
		}
		else if(m_direction == RIGHT && m_pSprite->m_sCurrentAnimation == "1_jump"+isLarge && m_fJumpBoostTimer != 0)
		{
			m_pSprite->PlayAnimation("1_idle", true);
		}
	}
}


void Player::Pickup(MysteryReward a_reward)
{
	if(a_reward == RED_MUSHROOM)
	{
		m_collisionRect.x = 32;
		m_collisionRect.y = 36;
		m_collisionRect.height = 60;
		m_collisionRect.width = 28;
		m_bIsLarge = true;
		m_pSprite->PlayAnimation(m_direction == LEFT ? "3_changesize" : "1_changesize", false);
		Sounds->Play("smb_powerup");
	}
	else if(a_reward == STAR)
	{
		g_bStarActive = true;
		Sounds->SetMusic("bgm_star",true);
	}
}


void Player::PullDownFlag(Entity* a_pFlagPole)
{
	m_flagPole = a_pFlagPole;
	m_playerState = PLAYER_LOWERING_FLAG;
	m_position.x = a_pFlagPole->GetPosition().x - 56;
	m_velocity.x = 0;
	std::string isLarge = m_bIsLarge ? "_large" : "";
	m_pSprite->PlayAnimation("pull_down"+isLarge,true);

}


void Player::EnterCastle()
{	
	m_velocity.x = 0;
	std::string isLarge = m_bIsLarge ? "_large" : "";
	m_playerState = PLAYER_ENTERING_CASTLE;
	m_pSprite->PlayAnimation("enter_castle"+isLarge);
}


void Player::EnterTube(Direction a_direction, Vector2 a_targetPosition, std::string a_sTargetLevel)
{
	m_tubeDirection = a_direction;
	m_tubeTargetPosition = a_targetPosition;
	m_tubeTargetLevel = a_sTargetLevel;
	m_fTubeDistanceTraveled = 0;
	m_playerState = PLAYER_IN_TUBE;
	Sounds->Play("smb_pipe");
	std::string isLarge = m_bIsLarge ? "_large" : "";

	m_velocity = Vector2();

	switch(m_tubeDirection)
	{
	case UP:
	case DOWN:
		m_pSprite->PlayAnimation("frontfacing"+isLarge, true);
		break;
	case RIGHT:
		m_pSprite->PlayAnimation("1_walk"+isLarge, true);		
		break;
	case LEFT:
		m_pSprite->PlayAnimation("3_walk"+isLarge, true);		
		break;
	}
}


void Player::Hurt()
{
	if(m_bIsVulnerable && !g_bStarActive)
	{
		if(m_bIsLarge)
		{
			m_bIsVulnerable = false;
			m_bIsLarge = false;
			Sounds->Play("smb2_shrink");
			m_pSprite->PlayAnimation(m_direction == LEFT ? "3_changesize" : "1_changesize", false);
			m_collisionRect.x = 34;
			m_collisionRect.y = 64;
			m_collisionRect.height = 32;
			m_collisionRect.width = 24;
		}
		else
		{
			Die();
		}
	}
}


void Player::Climb()
{
	if(m_playerState != PLAYER_CLIMBING)
	{
		m_playerState = PLAYER_CLIMBING;
		std::string isLarge = m_bIsLarge ? "_large" : "";
		sstream << m_direction;
		m_pSprite->PlayAnimation(sstream.str()+"_climb"+isLarge, true);
		sstream.str("");
		m_bUsesGravity = false;
		m_velocity = Vector2();
	}
	m_fClimbTimer = 0;
}