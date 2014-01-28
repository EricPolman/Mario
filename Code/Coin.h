#pragma once
#include "entity.h"

class Coin :
	public Entity
{
public:
	Coin(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Coin(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
};

