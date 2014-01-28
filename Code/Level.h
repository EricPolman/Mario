#pragma once
#include "includes.h"
#include "TileNode.h"

class Level
{
public:
	Level(std::string a_sName);
	~Level(void);

	std::string m_sName;
	Vector2 m_levelSize;
	
	void Init();
	void Update(float a_fDeltaTime, Vector2 a_cameraPosition);
	void Draw(Tmpl8::Surface* a_pTarget, Vector2 offset = Vector2(0,0), Vector2 a_cameraPosition = Vector2(0,0));
	void FillInTiles(Vector2 a_levelSize, std::vector<int>* a_idMap);
	std::vector<TileNode*> m_tiles;

private:
	void ParseEntityRow(std::vector<std::string>& a_row);
};

