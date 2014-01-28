#pragma once
#include "entity.h"
#include "Smashable.h"
#include "Bounceable.h"

class Goomba :
	public Entity, public Smashable, public Bounceable
{
public:
	Goomba(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	virtual ~Goomba(void);

	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	virtual void Update(float a_fDeltaTime);
	
	void Smash();
	void Hurt();
};