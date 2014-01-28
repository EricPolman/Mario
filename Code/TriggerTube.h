#pragma once
#include "trigger.h"
class TriggerTube :
	public Trigger
{
public:
	TriggerTube(float a_fX, float a_fY, Direction a_direction, std::string a_level, float a_fTargetX, float a_fTargetY);
	~TriggerTube(void);
	
	Direction m_direction;
	virtual bool OnTriggerEnter();
	virtual bool OnTriggerStay();
	virtual bool OnTriggerExit();

	std::string m_sTargetLevel;
	Vector2 m_targetPosition;

	bool m_bEntered;
};

