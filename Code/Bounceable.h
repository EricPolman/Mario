#pragma once
#include "includes.h"

class Bounceable
{
public:
	Bounceable(void){m_direction = RIGHT;}
	~Bounceable(void){}

	virtual void Bounce(){SwitchDirection();}
	inline Direction GetDirection() const {return m_direction;}
	inline void SetDirection(Direction a_direction) {m_direction = a_direction;}
	inline void SwitchDirection(){
		if(m_direction == RIGHT) m_direction = LEFT; 
		else m_direction = RIGHT;
	}
protected:
	Direction m_direction;
};

