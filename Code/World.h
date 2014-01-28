#pragma once
#include "includes.h"
#include "EntityManager.h"

enum WorldState{NORMAL, IN_TRANSITION};

class World
{
public:
	World(void);
	~World(void);

	void Init();
	void Update(float a_fDeltaTime);
	void Draw(Tmpl8::Surface* a_screen);
	void TestCollisions();

	Vector2 m_origin;
	Vector2 m_cameraPosition;
	Vector2 m_oldCameraPosition;

	Tmpl8::Surface* m_pWorldSurface;
	static Tmpl8::Surface* g_pScreen;

	int m_iCurrentLevel;
	float m_fTransitionTimer;

	static void ChangeLevel(std::string a_level, Player* a_pPlayer = 0, Vector2 a_targetPosition = Vector2(0,300));
	static Level* GetLevel(std::string a_level){return instance->m_pLevels[a_level];}
	static Level* GetCurrentLevel(){return instance->m_pCurrentLevel;}
	static void ResetCameraPositions(){instance->m_cameraPosition = Vector2(); instance->m_oldCameraPosition = Vector2();}
	
	std::string m_sCurrentLevel;

	WorldState m_state;

	void SetState(WorldState a_state);

	void PlacePlayer();
	void EndGame();
private:
	std::map<std::string, Level*> m_pLevels;
	Level* m_pCurrentLevel;

	CollisionRectangle GetTilesToCollideWith(CollisionRectangle a_ownRectangle);

	void PlaceEntities();

	static World* instance;	
};
