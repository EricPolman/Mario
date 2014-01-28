#pragma once
#include "entity.h"

class InvisibleWall :
	public Entity
{
public:
	InvisibleWall(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~InvisibleWall(void);
};

