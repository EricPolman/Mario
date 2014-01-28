#include "TriggerManager.h"

TriggerManager::~TriggerManager(void)
{
	instanceFlag = false;
	for(std::map<Level*, std::vector<Trigger*>>::iterator iter = m_triggersOfLevel.begin(); iter != m_triggersOfLevel.end(); iter++)
	{
		for(std::vector<Trigger*>::iterator i = iter->second.begin(); i != iter->second.end(); ++i )
		{
			delete *i;
		}
		iter->second.clear();
	}
}

bool TriggerManager::instanceFlag = false;
TriggerManager* TriggerManager::single = NULL;
TriggerManager* TriggerManager::GetInstance()
{
    if(! instanceFlag)
    {
        single = new TriggerManager();
		single->m_player = 0;
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}


void TriggerManager::Update(float a_fDeltaTime)
{
	for(std::vector<Trigger*>::iterator iter = m_triggersOfLevel[m_pCurrentLevel].begin(); iter != m_triggersOfLevel[m_pCurrentLevel].end(); iter++)
	{
		if(!(*iter)->Update(a_fDeltaTime, m_player))
		{
			break;
		}
	}
}


void TriggerManager::Add(Trigger* a_pTrigger)
{
	GetTriggers(m_pCurrentLevel)->push_back(a_pTrigger);
}


void TriggerManager::Add(Trigger* a_pTrigger, Level* a_room)
{
	GetTriggers(a_room)->push_back(a_pTrigger);
}


std::vector<Trigger*>* TriggerManager::GetTriggers(Level* a_room)
{
	return &m_triggersOfLevel[a_room];
}