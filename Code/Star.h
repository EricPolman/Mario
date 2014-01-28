#pragma once
#include "entity.h"
#include "Bounceable.h"

enum StarState {STAR_SPAWNING, STAR_MOVING};

class Star :
	public Entity, public Bounceable
{
public:
	Star(Direction a_direction, float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Star(void);

	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	virtual void Update(float a_fDeltaTime);

	StarState m_StarState;
	float m_fSpawnDistanceTraveled;
};

