#pragma once
#include "entity.h"
#include "Flag.h"

class Flagpole :
	public Entity
{
public:
	Flagpole(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Flagpole(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	
	Flag* m_pFlag;
	bool m_bFlagDown;
	bool m_bPullDownInitiated;

	void Update(float a_fDeltaTime);
};

