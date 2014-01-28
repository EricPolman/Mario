#pragma once
#include "entity.h"

class Flag :
	public Entity
{
public:
	Flag(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Flag(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	
	void Update(float a_fDeltaTime);
};

