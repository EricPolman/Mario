#include "TriggerWarp.h"
#include "TileManager.h"
#include "World.h"

TriggerWarp::TriggerWarp(float a_fX, float a_fY, std::string a_level, float a_fTargetX, float a_fTargetY)
	: Trigger(a_fX, a_fY)
{
	m_sTargetLevel = a_level;
	m_targetPosition = Vector2(a_fTargetX, a_fTargetY);
	m_bEntered = false;
	m_collisionRectangle.width = 64;
	m_collisionRectangle.x -= 16;
}


TriggerWarp::~TriggerWarp(void)
{
}


bool TriggerWarp::OnTriggerEnter()
{
	return true;
}


bool TriggerWarp::OnTriggerStay()
{
	if(!m_bEntered)
	{
		World::ChangeLevel(m_sTargetLevel, m_pPlayer, m_targetPosition);
		//m_pPlayer->EnterTube(m_direction, m_targetPosition, m_sTargetLevel);
		m_bEntered = true;
		return false;
	}
	return true;
}


bool TriggerWarp::OnTriggerExit()
{
	return true;
}
