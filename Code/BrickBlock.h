#pragma once
#include "entity.h"

class BrickBlock :
	public Entity
{
public:
	BrickBlock(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~BrickBlock(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	
	void Update(float a_fDeltaTime);

	float m_fBounceTimer;
	bool m_bIsBouncing;
	bool m_bIsShattering;
	Vector2 m_staticPosition;
};

