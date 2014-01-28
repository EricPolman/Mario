#include "EntityManager.h"
#include "TileManager.h"
#include "TriggerManager.h"

EntityManager::~EntityManager(void)
{
	instanceFlag = false;
	for(std::map<Level*, std::vector<Entity*>>::iterator iter = m_entitiesOfLevel.begin(); iter != m_entitiesOfLevel.end(); iter++)
	{
		for(std::vector<Entity*>::iterator i = iter->second.begin(); i != iter->second.end(); ++i )
		{
			delete *i;
		}
		iter->second.clear();
	}
	for(std::vector<Entity*>::iterator i = m_toBeRemoved.begin(); i != m_toBeRemoved.end(); ++i )
	{
		delete *i;
	}
	m_toBeRemoved.clear();
}

bool EntityManager::instanceFlag = false;
EntityManager* EntityManager::single = NULL;
EntityManager* EntityManager::GetInstance()
{
    if(! instanceFlag)
    {
        single = new EntityManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}


void EntityManager::Add(Entity* a_pEntity)
{
	m_entityQueue[m_pCurrentLevel].push(a_pEntity);
	//GetEntities(m_pCurrentLevel)->push_back(a_pEntity);
}


void EntityManager::Add(Entity* a_pEntity, Level* a_room)
{
	m_entityQueue[a_room].push(a_pEntity);
	//GetEntities(m_pCurrentLevel)->push_back(a_pEntity);
}


void EntityManager::Remove(Entity* a_pEntity)
{
	for(std::vector<Entity*>::iterator i = GetEntities(m_pCurrentLevel)->begin(); i != GetEntities(m_pCurrentLevel)->end(); i++)
	{
		if(*i == a_pEntity)
		{
			GetEntities(m_pCurrentLevel)->erase(i);
			break;
		}
	}
}

void EntityManager::ClearAll()
{
	for(std::map<Level*, std::vector<Entity*>>::iterator iter = m_entitiesOfLevel.begin(); iter != m_entitiesOfLevel.end(); iter++)
	{
		for(std::vector<Entity*>::iterator i = iter->second.begin(); i != iter->second.end(); ++i )
		{
			delete *i;
		}
		iter->second.clear();
	}
}

void EntityManager::ClearLevel(Level* a_level)
{
	for(std::vector<Entity*>::iterator i = m_entitiesOfLevel[a_level].begin(); i != m_entitiesOfLevel[a_level].end(); ++i )
	{
		(*i)->m_state = REMOVED;
	}
	//m_entitiesOfLevel[a_level].clear();
	m_playerPerLevel[a_level] = 0;
}

void EntityManager::Update(float a_fDeltaTime, Level* a_room, Vector2 a_cameraPosition)
{
	if(Input->IsKeyPressed(SDLK_m))
	{
		SpawnPlayer(m_pCurrentLevel);
	}
	while(!m_entityQueue[a_room].empty())
	{
		GetEntities(a_room)->push_back(m_entityQueue[a_room].front());
		m_entityQueue[a_room].pop();
	}
	
	std::sort(GetEntities(a_room)->begin(), GetEntities(a_room)->end(), EntityIsAboveOther());

	for(std::vector<Entity*>::iterator i = GetEntities(a_room)->begin(); i != GetEntities(a_room)->end();)
	{
		Entity* iEntity = *i;
		Vector2 pos = iEntity->GetPosition();
		if( pos.x + 96 < a_cameraPosition.x ||
			pos.x - 96 > a_cameraPosition.x + SCRWIDTH	)
		{
			i++;
			continue;
		}
		else
		{
			iEntity->Update(a_fDeltaTime);
		}

		if(iEntity->m_state == EntityState::REMOVED)
		{
			m_toBeRemoved.push_back(*i);
			i = GetEntities(a_room)->erase(i);
			continue;
		}
		else
		{
			i++;
		}
	}

	if(m_toBeRemoved.size() > 0)
	{
		for(std::vector<Entity*>::iterator i = m_toBeRemoved.begin(); i != m_toBeRemoved.end(); ++i )
		{
			delete *i;
		}
		m_toBeRemoved.clear();
	}
}


void EntityManager::Draw(Tmpl8::Surface* a_surface, Vector2 a_origin, Level* a_room, Vector2 a_cameraPosition)
{
	for(std::vector<Entity*>::iterator i = GetEntities(a_room)->begin(); i != GetEntities(a_room)->end(); i++)
	{
		Vector2 pos = (*i)->GetPosition();
		if( pos.x + 32 < a_cameraPosition.x ||
			pos.x - 32 > a_cameraPosition.x + SCRWIDTH	)
		{
			continue;
		}

		(*i)->Draw(a_surface, a_origin);
	}
}


std::vector<Entity*>* EntityManager::GetEntities(Level* a_room)
{
	return &m_entitiesOfLevel[a_room];
}


void EntityManager::PushQueues()
{
	Level* level = World::GetCurrentLevel();
	while(!m_entityQueue[level].empty())
	{
		GetEntities(level)->push_back(m_entityQueue[level].front());
		m_entityQueue[level].pop();
	}
}


void EntityManager::SpawnPlayer(Level* a_pLevel, Player* a_pPlayer, Vector2 a_targetPosition)
{
	if(a_pLevel == 0)
	{
		a_pLevel = m_pCurrentLevel;
	}
	
	if(m_playerPerLevel[a_pLevel] != 0)
	{
		m_playerPerLevel[a_pLevel]->m_state = REMOVED;
	}
	if(a_pPlayer == 0)
	{
		m_playerPerLevel[a_pLevel] = new Player("player",a_pLevel->m_sName,a_targetPosition.x,a_targetPosition.y);
		m_playerPerLevel[a_pLevel]->Spawn();
	}
	else
	{
		m_playerPerLevel[a_pLevel] = new Player(*a_pPlayer);
		m_playerPerLevel[a_pLevel]->SetPosition(a_targetPosition);
	}
	Triggers->m_player = m_playerPerLevel[a_pLevel];
}