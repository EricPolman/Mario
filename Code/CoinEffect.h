#pragma once
#include "entity.h"

class CoinEffect :
	public Entity
{
public:
	CoinEffect(float a_fX = 0, float a_fY = 0, Level* a_level = 0);
	~CoinEffect(void);
	
	virtual void Update(float a_fDeltaTime);
};

