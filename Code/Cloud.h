#pragma once
#include "entity.h"

class Cloud :
	public Entity
{
public:
	Cloud(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Cloud(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	
	void Update(float a_fDeltaTime);

	bool m_bIsMoving;
};

