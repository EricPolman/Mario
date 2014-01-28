#pragma once
#include "entity.h"

enum MysteryReward{COINS, RED_MUSHROOM, GREEN_MUSHROOM, STAR, FIRE_FLOWER};

class MysteryCube :
	public Entity
{
public:
	MysteryCube(MysteryReward a_reward = COINS, float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~MysteryCube(void);
	
	MysteryReward m_reward;

	float m_fBounceTimer;
	bool m_bIsBouncing;

	void Update(float a_fDeltaTime);

	Vector2 m_staticPosition;
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
};

