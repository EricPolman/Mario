#pragma once
class Smashable
{
public:
	Smashable(void){m_bIsSmashed = false;}
	~Smashable(void){}
	
	bool m_bIsSmashed;
	virtual void Smash() = 0;
};

