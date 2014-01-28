#pragma once
#include "trigger.h"
class TriggerEnterCastle :
	public Trigger
{
public:
	TriggerEnterCastle(float a_fX, float a_fY, std::string a_level, float a_fTargetX, float a_fTargetY);
	~TriggerEnterCastle(void);
	
	virtual bool OnTriggerEnter();
	virtual bool OnTriggerStay();
	virtual bool OnTriggerExit();

	std::string m_sTargetLevel;
	Vector2 m_targetPosition;
};

