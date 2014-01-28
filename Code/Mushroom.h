#pragma once
#include "entity.h"
#include "Bounceable.h"

enum MushroomState {MUSHROOM_SPAWNING, MUSHROOM_MOVING};

class Mushroom :
	public Entity, public Bounceable
{
public:
	Mushroom(Direction a_direction, float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~Mushroom(void);

	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	virtual void Update(float a_fDeltaTime);

	MushroomState m_mushroomState;
	float m_fSpawnDistanceTraveled;
};

