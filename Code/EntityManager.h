#pragma once
#include "Player.h"
#include "World.h"

class EntityManager
{
public:
	~EntityManager(void);
	static EntityManager* GetInstance(void);

	void Update(float a_fDeltaTime, Level* a_room, Vector2 a_cameraPosition);
	void Draw(Tmpl8::Surface* a_screen, Vector2 a_origin, Level* a_room, Vector2 a_cameraPosition);
	void Remove(Entity* a_pEntity);
	void Add(Entity* a_pEntity);
	void Add(Entity* a_pEntity, Level* a_room);
	std::vector<Entity*>* GetEntities(Level* a_room);
	std::queue<Entity*>* GetEntityQueue(Level* a_room){return &m_entityQueue[a_room];}
	void PushQueues();
	void ClearAll();
	void ClearLevel(Level* a_level);
	Player* GetPlayer(){return m_playerPerLevel[m_pCurrentLevel];}
	
	void AddEntityVectorForLevel(Level* a_room)
	{
		std::map<Level*, std::vector<Entity*>>::iterator i = m_entitiesOfLevel.find(a_room);
		if (i == m_entitiesOfLevel.end()) { 
			m_entitiesOfLevel[a_room] = std::vector<Entity*>();
		}
		std::map<Level*, std::queue<Entity*>>::iterator j = m_entityQueue.find(a_room);
		if (j == m_entityQueue.end()) { 
			m_entityQueue[a_room] = std::queue<Entity*>(); 
		}
	}

	void SetCurrentLevel(Level* a_room){m_pCurrentLevel = a_room;}
	void SetPlayer(Player* a_pPlayer);
	void SpawnPlayer(Level* a_pLevel, Player* a_pPlayer = 0, Vector2 a_targetPosition = Vector2(0,300));
private:
	std::vector<Entity*> m_toBeRemoved;
	static bool instanceFlag;

	//std::vector<Entity*> m_entities;
	std::map<Level*, std::vector<Entity*>> m_entitiesOfLevel;
	std::map<Level*, std::queue<Entity*>> m_entityQueue;
	std::map<Level*, Player*> m_playerPerLevel;

	static EntityManager *single;
	EntityManager(void){}

	struct EntityIsAboveOther
	{
		bool operator()( Entity* entity1, Entity* entity2 ) const {
			return entity1->m_zIndex < entity2->m_zIndex;
		}
	};

	Level* m_pCurrentLevel;
};


#define Entities EntityManager::GetInstance()