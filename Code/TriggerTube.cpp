#include "TriggerTube.h"
#include "TileManager.h"


TriggerTube::TriggerTube(float a_fX, float a_fY, Direction a_direction, std::string a_level, float a_fTargetX, float a_fTargetY)
	: Trigger(a_fX, a_fY)
{
	m_sTargetLevel = a_level;
	m_targetPosition = Vector2(a_fTargetX, a_fTargetY);
	m_direction = a_direction;
	m_bEntered = false;
}


TriggerTube::~TriggerTube(void)
{
}


bool TriggerTube::OnTriggerEnter()
{
	return true;
}


bool TriggerTube::OnTriggerStay()
{
	if(!m_bEntered)
	{
		switch(m_direction)
		{
		case DOWN:
			if(Input->IsKeyDown(SDLK_s) || Input->IsKeyDown(SDLK_DOWN))
			{
				m_pPlayer->EnterTube(m_direction, m_targetPosition, m_sTargetLevel);
				m_bEntered = true;
			}
			break;
		case RIGHT:
			if(Input->IsKeyDown(SDLK_d) || Input->IsKeyDown(SDLK_RIGHT))
			{
				m_pPlayer->EnterTube(m_direction, m_targetPosition, m_sTargetLevel);
				m_bEntered = true;
			}
			break;
		case UP:
			if(Input->IsKeyDown(SDLK_w) || Input->IsKeyDown(SDLK_UP))
			{			
				m_pPlayer->EnterTube(m_direction, m_targetPosition, m_sTargetLevel);
				m_bEntered = true;
			}
			break;
		case LEFT:
			if(Input->IsKeyDown(SDLK_a) || Input->IsKeyDown(SDLK_LEFT))
			{
				m_pPlayer->EnterTube(m_direction, m_targetPosition, m_sTargetLevel);
				m_bEntered = true;
			}
			break;
		}
	}
	return true;
}


bool TriggerTube::OnTriggerExit()
{
	return true;
}