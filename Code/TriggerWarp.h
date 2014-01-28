#pragma once
#include "trigger.h"
class TriggerWarp :
	public Trigger
{
public:
	TriggerWarp(float a_fX, float a_fY, std::string a_level, float a_fTargetX, float a_fTargetY);
	~TriggerWarp(void);
	
	virtual bool OnTriggerEnter();
	virtual bool OnTriggerStay();
	virtual bool OnTriggerExit();

	std::string m_sTargetLevel;
	Vector2 m_targetPosition;

	bool m_bEntered;
};

