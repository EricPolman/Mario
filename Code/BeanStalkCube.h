#pragma once
#include "entity.h"
#include "BeanStalkPart.h"

class BeanStalkCube :
	public Entity
{
public:
	BeanStalkCube(std::string a_destination, float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~BeanStalkCube(void);
	
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	
	void Update(float a_fDeltaTime);
	
	std::string m_destination;
	bool m_bIsUsed;
	bool m_bBeanStalkGrown;

	float m_fMoveTimer;

private:
	std::vector<BeanStalkPart*> m_beanstalkParts;
};

