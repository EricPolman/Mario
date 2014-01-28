#pragma once
#include "includes.h"
#include "Level.h"
#include "TileSurface.h"
#include "TileNode.h"
#include "World.h"

class TileManager
{
public:
	~TileManager(void);
	static TileManager* getInstance(void);

	void LoadBaseSprite(std::string a_sFileName, std::string a_sPath);
	void LoadExistingMap(std::string a_sFileName, Level* a_level);
	void ProcessTileMap(std::string a_sFileName);
	TileSurface* GetTileSurface(std::string a_sFileName, int id);
	TileSurface* GetTileSurface(int id);
	
	World* m_pWorld;
	std::string m_sCurrentLevel;
private:
	Tmpl8::Surface* surface;

	//Level* m_rooms[MAP_WIDTH / ROOM_WIDTH * MAP_HEIGHT / ROOM_HEIGHT];
	//Level* m_pCurrentLevel;

	static bool instanceFlag;
	static TileManager *single;
	TileManager(void);

	std::map<std::string, Tmpl8::Surface*> m_pBaseSprite;
	std::map<std::string, std::vector<TileSurface*>> m_pTileSurfaces;
	std::map<std::string, std::vector<TileNode*>> m_pTiles;
	std::map<std::string, Vector2> m_levelSize;
	
	void LoadTileSurfaces(std::string a_sFileName, int a_tileWidth, int a_tileHeight, int a_width, int a_height, int a_amountOfTiles);
	void ComposeMap(std::string a_sFileName, Level* a_level);
};

#define Tiles TileManager::getInstance()