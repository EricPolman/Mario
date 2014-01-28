#include "TileManager.h"
#include "freeimage.h"
#include "TriggerManager.h"

TileManager::TileManager(void)
{
}

TileManager::~TileManager(void)
{
	instanceFlag = false;
	//delete m_pBaseSprite;
}

bool TileManager::instanceFlag = false;
TileManager* TileManager::single = NULL;
TileManager* TileManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new TileManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}


void TileManager::LoadBaseSprite(std::string a_sFileName, std::string a_sPath)
{
	m_sCurrentLevel = a_sFileName;
	m_pBaseSprite[a_sFileName] = new Tmpl8::Surface(a_sPath.c_str());
	m_levelSize[a_sFileName] = Vector2(m_pBaseSprite[a_sFileName]->GetWidth(), m_pBaseSprite[a_sFileName]->GetHeight());
}


void TileManager::ProcessTileMap(std::string a_sFileName)
{
	m_sCurrentLevel = a_sFileName;
	int rows = m_pBaseSprite[a_sFileName]->GetHeight() / TILE_HEIGHT;
	int columns = m_pBaseSprite[a_sFileName]->GetWidth() / TILE_WIDTH;
	int idCounter = 0;
	
	int id = 0;

	int counter = 0;
	
	for(int row = 0; row < rows; row++)
	{
		for(int col = 0; col < columns; col++)
		{
			Tmpl8::Surface* surface = new Tmpl8::Surface(TILE_WIDTH, TILE_HEIGHT);
			m_pBaseSprite[a_sFileName]->CopyPartTo(surface, 0, 0, col * TILE_WIDTH, row * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

			if(row == 0 && col == 0)
			{
				TileNode* tileNode = new TileNode(row, col, 0);
				m_pTiles[a_sFileName].push_back(tileNode);
				m_pTileSurfaces[a_sFileName].push_back(new TileSurface(idCounter,surface));
				tileNode->SetSurface(tileNode->m_iTileId);
				counter++;
				idCounter++;
				continue;
			}
			bool isUnique = true;
			for(std::vector<TileSurface*>::iterator iter = m_pTileSurfaces[a_sFileName].begin(); iter != m_pTileSurfaces[a_sFileName].end();)
			{
				if(!(*iter)->CompareSurfaces(surface))
				{
					isUnique = false;
					id = (*iter)->m_iTileId;
					iter = m_pTileSurfaces[a_sFileName].end();
				}
				else
				{
					iter++;
				}
			}

			if(isUnique)
			{
				m_pTileSurfaces[a_sFileName].push_back(new TileSurface(idCounter,surface));
				id = idCounter;
				idCounter++;
			}
			else
			{
				delete surface;
			}
			TileNode* tileNode = new TileNode(row, col, id);
			m_pTiles[a_sFileName].push_back(tileNode);
			tileNode->SetSurface(tileNode->m_iTileId);
			counter++;
		}
	}
	FIBITMAP* bitmap = FreeImage_Allocate(512, 1024, 24);
	int tilesInRow = 512 / TILE_WIDTH;
	int rowCounter = 0;
	int colCounter = 0;
	for(std::vector<TileSurface*>::iterator iter = m_pTileSurfaces[a_sFileName].begin(); iter != m_pTileSurfaces[a_sFileName].end(); iter++)
	{
		Tmpl8::Pixel* buffer = (*iter)->m_pTileSurface->GetBuffer();
		int pitch = (*iter)->m_pTileSurface->GetPitch();

		for(int x = 0; x < TILE_WIDTH; x++)
		{
			for(int y = 0; y < TILE_HEIGHT; y++)
			{
				RGBQUAD color;
				color.rgbRed = (buffer[x + y*pitch] & 0xFF0000) >> 16;
				color.rgbGreen = (buffer[x + y*pitch] & 0x00FF00) >> 8;
				color.rgbBlue = buffer[x + y*pitch] & 0x0000FF;
				FreeImage_SetPixelColor(bitmap, colCounter*TILE_WIDTH + x, 1023 - rowCounter*TILE_HEIGHT - y, &color);
			}
		}
		colCounter++;
		if(colCounter >= tilesInRow)
		{
			colCounter = 0;
			rowCounter++;
		}
	}
	FreeImage_Save(FIF_PNG, bitmap, std::string("assets/tilesets/"+a_sFileName+"_tilemap.png").c_str(), 0);

	std::string tileMap;
	std::stringstream ss;
			
	counter = 0;
	for(int row = 0; row < rows; row++)
	{
		for(int col = 0; col < columns; col++)
		{
			TileNode* currentNode = m_pTiles[a_sFileName][counter];
			counter++;

			ss.str("");
			ss << currentNode->m_iTileId;
			tileMap += ss.str();

			if(col+1 != columns)
				tileMap += ",";
		}
		
		if(row+1 != rows)
			tileMap += "\n";
	}
	std::ofstream tilemapFile;
	tilemapFile.open (std::string("assets/tilesets/"+a_sFileName+".dat").c_str());
	tilemapFile << tileMap;
	tilemapFile.close();
	std::ofstream metaFile;
	metaFile.open (std::string("assets/tilesets/"+a_sFileName+"_metadata.dat").c_str());
	ss.str("");
	ss << m_pTileSurfaces[a_sFileName].size() << "," << TILE_WIDTH << "," << TILE_HEIGHT << "," << m_pBaseSprite[a_sFileName]->GetWidth() << "," << m_pBaseSprite[a_sFileName]->GetHeight();
	metaFile << ss.str();
	metaFile.close();
	
	for(std::vector<TileSurface*>::iterator iter = m_pTileSurfaces[a_sFileName].begin(); iter != m_pTileSurfaces[a_sFileName].end(); iter++)
	{
		delete *iter;
	}
	m_pTileSurfaces[a_sFileName].clear();
	for(std::vector<TileNode*>::iterator iter = m_pTiles[a_sFileName].begin(); iter != m_pTiles[a_sFileName].end(); iter++)
	{
		delete *iter;
	}
	m_pTiles[a_sFileName].clear();
	delete m_pBaseSprite[a_sFileName];
}


TileSurface* TileManager::GetTileSurface(std::string a_sFileName, int id)
{
	return m_pTileSurfaces[a_sFileName][id];
}
TileSurface* TileManager::GetTileSurface(int id)
{
	return m_pTileSurfaces[m_sCurrentLevel][id];
}

void TileManager::LoadExistingMap(std::string a_sFileName, Level* a_level)
{
	m_sCurrentLevel = a_sFileName;
	std::ifstream ifile(std::string("assets/tilesets/"+a_sFileName+"_metadata.dat").c_str());
	std::string metadataRaw;
	std::vector<std::string> metadata;
	
	if (ifile) {
		getline (ifile,metadataRaw);
		ifile.close();
	}
	metadata = explode(metadataRaw, ',');
	m_levelSize[a_sFileName].x = atoi(metadata[3].c_str());
	m_levelSize[a_sFileName].y = atoi(metadata[4].c_str());
	LoadTileSurfaces(a_sFileName, atoi(metadata[1].c_str()), atoi(metadata[2].c_str()), atoi(metadata[3].c_str()), atoi(metadata[4].c_str()), atoi(metadata[0].c_str()));
	ComposeMap(a_sFileName, a_level);
}

void TileManager::LoadTileSurfaces(std::string a_sFileName, int a_tileWidth, int a_tileHeight, int a_width, int a_height, int a_amountOfTiles)
{
	int counter = 0;
	Tmpl8::Surface* tileset = new Tmpl8::Surface(std::string("assets/tilesets/"+a_sFileName+"_tilemap.png").c_str());
	Tmpl8::Surface* collisionMap = new Tmpl8::Surface(std::string("assets/tilesets/"+a_sFileName+"_collision.png").c_str());
	Tmpl8::Pixel* collisionBuffer = collisionMap->GetBuffer();
	
	for(int y = 0; y < tileset->GetHeight() / a_tileHeight; y++)
	{
		for(int x = 0; x < tileset->GetWidth() / a_tileWidth; x++)
		{
			Tmpl8::Surface* tile = new Tmpl8::Surface(a_tileWidth, a_tileHeight);
			tileset->CopyPartTo(
				tile, 0,0,x * a_tileWidth,
				y * a_tileHeight, 
				a_tileWidth, 
				a_tileHeight);
			TileSurface* tSurface = new TileSurface(counter, tile);
			m_pTileSurfaces[a_sFileName].push_back(tSurface);
			switch(collisionBuffer[x + y*tileset->GetWidth()/a_tileWidth])
			{
			case 0xFF000000:						//Black
				tSurface->m_collisionType = NONE;
				break;
			case 0xFFFFFFFF:						//White
				tSurface->m_collisionType = FULL;
				break;
			case 0xFFFF0000:						//Red
				tSurface->m_collisionType = HALF_LEFT;
				break;
			case 0xFF00FF00:						//Green
				tSurface->m_collisionType = HALF_RIGHT;
				break;
			case 0xFF0000FF:						//Blue
				tSurface->m_collisionType = HALF_TOP;
				break;
			case 0xFFFFFF00:						//Yellow
				tSurface->m_collisionType = HALF_BOTTOM;
				break;
			default:
				tSurface->m_collisionType = NONE;
				break;
			}
			
			counter++;
			if(counter == a_amountOfTiles)
			{
				return;
			}
		}
	}
	delete tileset;
}


void TileManager::ComposeMap(std::string a_sFileName, Level* a_level)
{
	m_sCurrentLevel = a_sFileName;
	std::ifstream ifile(std::string("assets/tilesets/"+a_sFileName+".dat").c_str());
	std::vector<std::string> tileMapRaw;
	
	if (ifile) {
		std::string temp = "";
		while(getline(ifile,temp)){
			tileMapRaw.push_back(temp);
			std::string temp = "";
		}
		ifile.close();
	}

	int xCounter = 0;
	int yCounter = 0;

	std::vector<int> idMap;
	
	for(std::vector<std::string>::iterator iter = tileMapRaw.begin(); iter != tileMapRaw.end(); iter++)
	{
		std::vector<std::string> tileIds = explode(*iter, ',');
		for(std::vector<std::string>::iterator tileIter = tileIds.begin(); tileIter != tileIds.end(); tileIter++)
		{
			idMap.push_back(std::atoi(tileIter->c_str()));
			xCounter++;
		}
		xCounter = 0;
		yCounter++;
	}

	a_level->FillInTiles(m_levelSize[a_sFileName],&idMap);
	
	Entities->AddEntityVectorForLevel(m_pWorld->GetLevel(a_sFileName));
}