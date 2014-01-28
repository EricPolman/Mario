#include "Level.h"
#include "EntityManager.h"
#include "TileManager.h"
#include "TriggerManager.h"
#include "MysteryCube.h"
#include "BrickBlock.h"
#include "Mushroom.h"
#include "TriggerTube.h"
#include "Flagpole.h"
#include "Coin.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "TriggerEnterCastle.h"
#include "TriggerWarp.h"
#include "BeanStalkCube.h"
#include "InvisibleWall.h"
#include "Cloud.h"
#include <assert.h>

Level::Level(std::string a_sName)
{
	m_sName = a_sName;

	std::ifstream ifile("assets/tilesets/"+a_sName+"_metadata.dat");
	if (!ifile) {
		Tiles->LoadBaseSprite(a_sName, "assets/backgrounds/"+a_sName+".png");
		Tiles->ProcessTileMap(a_sName);
	}
	Tiles->LoadExistingMap(a_sName, this);
}

Level::~Level(void)
{
}


void Level::Init()
{
	std::ifstream ifile(std::string("assets/leveldata/"+m_sName+".dat").c_str());
	std::vector<std::string> entityRows;
	
	if (ifile) {
		std::string temp = "";
		while(getline(ifile,temp)){
			entityRows.push_back(temp);
			std::string temp = "";
		}
		ifile.close();
	}
	else
	{
		assert(false);
		//exit(1);
	}

	for(std::vector<std::string>::iterator iter = entityRows.begin(); iter != entityRows.end(); iter++)
	{
		std::vector<std::string> entityRow = explode(*iter, ',');
		ParseEntityRow(entityRow);
	}

	Entities->PushQueues();
}


void Level::Update(float a_fDeltaTime, Vector2 a_cameraPosition)
{
	if(Entities->GetPlayer()->GetPosition().x < a_cameraPosition.x - 16) 
		Entities->GetPlayer()->SetPosition(Vector2(a_cameraPosition.x-16, Entities->GetPlayer()->GetPosition().y));
	
	Entities->Update(a_fDeltaTime, this, a_cameraPosition);
	Triggers->Update(a_fDeltaTime);	
}


void Level::Draw(Tmpl8::Surface* a_pTarget, Vector2 offset, Vector2 a_cameraPosition)
{
	int leftBound = a_cameraPosition.x / TILE_WIDTH;
	if(leftBound < 0) leftBound = 0; 
	int rightBound = (a_cameraPosition.x / TILE_WIDTH + SCRWIDTH / TILE_WIDTH) + 1;
	if(rightBound > m_levelSize.x / TILE_WIDTH)
		rightBound = m_levelSize.x / TILE_WIDTH;

	int topBound = a_cameraPosition.y / TILE_HEIGHT;
	if(topBound < 0) topBound = 0; 
	int bottomBound = (a_cameraPosition.y / TILE_HEIGHT + SCRHEIGHT / TILE_HEIGHT) + 1;
	if(bottomBound > m_levelSize.y / TILE_HEIGHT)
		bottomBound = m_levelSize.y / TILE_HEIGHT;


	for(int tileY = topBound; tileY < bottomBound; tileY++)
	{
		for(int tileX = leftBound; tileX < rightBound; tileX++)
		{
			m_tiles[tileX + tileY * (m_levelSize.x / TILE_WIDTH)]->Draw(a_pTarget, Vector2());
		}
	}
}


void Level::FillInTiles(Vector2 a_levelSize, std::vector<int>* a_idMap)
{
	m_levelSize = a_levelSize;
	m_tiles.reserve(a_levelSize.x / TILE_WIDTH * a_levelSize.y / TILE_HEIGHT);

	for(int tileY = 0; tileY < a_levelSize.y / TILE_HEIGHT; tileY++)
	{
		for(int tileX = 0; tileX < a_levelSize.x / TILE_WIDTH; tileX++)
		{
			int index = tileX + tileY * (a_levelSize.x / TILE_WIDTH);
			TileNode* t = new TileNode(tileY, tileX, (*a_idMap)[index]);
			m_tiles.push_back(t);
			t->SetSurface(t->m_iTileId);
		}
	}
}


void Level::ParseEntityRow(std::vector<std::string>& a_row)
{
	if(a_row[0] == "MysteryCube")
	{
		new MysteryCube((MysteryReward)atoi(a_row[3].c_str()), atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "BrickBlock")
	{
		new BrickBlock(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "PipeUp")
	{
		Entity* e = new Entity("pipe_up", atoi(a_row[1].c_str()), atoi(a_row[2].c_str()), 0, atoi(a_row[4].c_str()), atoi(a_row[5].c_str()));
		e->m_zIndex = 5;
	}
	else if(a_row[0] == "PipeLeft")
	{
		Entity* e = new Entity("pipe_left", atoi(a_row[1].c_str()), atoi(a_row[2].c_str()), 0, atoi(a_row[4].c_str()), atoi(a_row[5].c_str()));
		e->m_zIndex = 5;
	}
	else if(a_row[0] == "Flagpole")
	{
		new Flagpole(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "TriggerTube")
	{
		Triggers->Add(new TriggerTube(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()), (Direction)atoi(a_row[3].c_str()), a_row[4], atoi(a_row[5].c_str()), atoi(a_row[6].c_str())));
	}
	else if(a_row[0] == "TriggerEnterCastle")
	{
		Triggers->Add(new TriggerEnterCastle(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()), a_row[3], 100, 300));
	}
	else if(a_row[0] == "TriggerWarp")
	{
		TriggerWarp* t = new TriggerWarp(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()), a_row[3], atoi(a_row[4].c_str()), atoi(a_row[5].c_str()));
		t->m_collisionRectangle.width = atoi(a_row[6].c_str());
		t->m_collisionRectangle.height = atoi(a_row[7].c_str());
		Triggers->Add(t);
	}
	else if(a_row[0] == "Coin")
	{
		new Coin(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "Goomba")
	{
		new Goomba(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "KoopaTroopa")
	{
		new KoopaTroopa(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "BeanStalkCube")
	{
		new BeanStalkCube(a_row[3], atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "InvisibleWall")
	{
		new InvisibleWall(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
	else if(a_row[0] == "Cloud")
	{
		new Cloud(atoi(a_row[1].c_str()), atoi(a_row[2].c_str()));
	}
}