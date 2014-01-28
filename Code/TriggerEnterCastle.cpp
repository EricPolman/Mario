#include "TriggerEnterCastle.h"
#include "TileManager.h"


TriggerEnterCastle::TriggerEnterCastle(float a_fX, float a_fY, std::string a_level, float a_fTargetX, float a_fTargetY)
	: Trigger(a_fX, a_fY)
{
	m_sTargetLevel = a_level;
	m_targetPosition = Vector2(a_fTargetX, a_fTargetY);
}


TriggerEnterCastle::~TriggerEnterCastle(void)
{
}


bool TriggerEnterCastle::OnTriggerEnter()
{
	m_pPlayer->EnterCastle();
	return true;
}


bool TriggerEnterCastle::OnTriggerStay()
{
	return true;
}


bool TriggerEnterCastle::OnTriggerExit()
{
	return true;
}