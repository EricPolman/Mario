#pragma once
#include "entity.h"
#include "Smashable.h"
#include "Bounceable.h"

class KoopaTroopa :
	public Entity, public Smashable, public Bounceable
{
public:
	KoopaTroopa(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	virtual ~KoopaTroopa(void);

	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	virtual void Update(float a_fDeltaTime);
	
	void Smash();
	void Bounce();
private:
	float m_fInvulnerableTimer;
	bool m_bIsVulnerable;
	bool m_bDashing;
};
