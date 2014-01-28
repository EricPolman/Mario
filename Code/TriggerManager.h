#pragma once
#include "Trigger.h"

class TriggerManager
{
public:
	~TriggerManager(void);
	static TriggerManager* GetInstance(void);
	
	void Update(float a_fDeltaTime);

	void Add(Trigger* a_pTrigger);
	void Add(Trigger* a_pTrigger, Level* a_room);
	std::vector<Trigger*>* GetTriggers(Level* a_room);

	Level* m_pCurrentLevel;
	std::map<Level*, std::vector<Trigger*>> m_triggersOfLevel;
	Player* m_player;
private:
	static bool instanceFlag;

	static TriggerManager *single;
	TriggerManager(void){}
};


#define Triggers TriggerManager::GetInstance()