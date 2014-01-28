#include "World.h"
#include "TileManager.h"
#include "TextureManager.h"
#include "TriggerManager.h"
#include "UserInterface.h"
#include "Player.h"
#include "Bounceable.h"

UserInterface* m_pUserInterface = 0;
Tmpl8::Surface* World::g_pScreen = 0;
World* World::instance = 0;

World::World(void)
{
	m_origin = Vector2(0,0);
	m_pWorldSurface = new Tmpl8::Surface(7000, 512);
	m_state = NORMAL;
	m_fTransitionTimer = 0;
	g_pScreen = 0;
	instance = this;
}


World::~World(void)
{
}


void World::Init()
{
	Textures->LoadTextures();
	
	Tiles->m_pWorld = this;
	
	m_pLevels["level_1-1-hidden"] = new Level("level_1-1-hidden");
	m_pLevels["level_2-1-underground"] = new Level("level_2-1-underground");
	m_pLevels["level_2-1-skies"] = new Level("level_2-1-skies");
	m_pLevels["level_2-1"] = new Level("level_2-1");
	m_pLevels["level_1-1"] = new Level("level_1-1");

	ChangeLevel("level_1-1");

	m_pUserInterface = new UserInterface();
	m_pUserInterface->SetPlayer(0);
}

float flashTimer = 0;
int flashCounter;
void World::Update(float a_fDeltaTime)
{
	m_pCurrentLevel->Update(a_fDeltaTime, m_cameraPosition);
	TestCollisions();
	if(Player::g_bStarActive)
	{
		flashTimer += a_fDeltaTime;
		if(flashTimer > 0.1)
		{
			flashTimer -= 0.1;
			flashCounter++;
		}
	}

	if(Entities->GetPlayer()->m_bClearedLevel || Entities->GetPlayer()->m_bDied)
	{
		Entities->GetPlayer()->m_state = REMOVED;
		if(Entities->GetPlayer()->m_bDied)
		{
			ChangeLevel(m_sCurrentLevel);
		}
		else
		{
			if(m_sCurrentLevel == "level_1-1")
			{
				ChangeLevel("level_2-1");
			}
			else
			{
				ChangeLevel("level_1-1");
			}
		}

		m_oldCameraPosition = Vector2(0,0);
		m_cameraPosition.x = Entities->GetPlayer()->GetPosition().x + Entities->GetPlayer()->GetSprite()->m_textureRect.width - SCRWIDTH/2;
		m_cameraPosition.y = Entities->GetPlayer()->GetPosition().y + Entities->GetPlayer()->GetSprite()->m_textureRect.height - SCRHEIGHT/2;	
	}
	m_oldCameraPosition = m_cameraPosition;
	m_cameraPosition.x = 
		m_cameraPosition.x * 0.9f + 
		(Entities->GetPlayer()->GetPosition().x + Entities->GetPlayer()->GetSprite()->m_textureRect.width - SCRWIDTH/2) * 0.1f;
	m_cameraPosition.y = Entities->GetPlayer()->GetPosition().y + Entities->GetPlayer()->GetSprite()->m_textureRect.height - SCRHEIGHT/2;
	if(m_cameraPosition.x < m_oldCameraPosition.x) m_cameraPosition.x = m_oldCameraPosition.x;

	if(m_cameraPosition.x + SCRWIDTH > GetCurrentLevel()->m_levelSize.x)
	{
		m_cameraPosition.x = GetCurrentLevel()->m_levelSize.x - SCRWIDTH;
	}
	if(m_cameraPosition.x < 0)
	{
		m_cameraPosition.x = 0;
	}
	if(m_cameraPosition.y < 0)
	{
		m_cameraPosition.y = 0;
	}
	if(m_cameraPosition.y + SCRHEIGHT > GetCurrentLevel()->m_levelSize.y)
	{
		m_cameraPosition.y =  GetCurrentLevel()->m_levelSize.y - SCRHEIGHT;
	}
}

void World::Draw(Tmpl8::Surface* a_screen)
{
	if(g_pScreen == 0)
	{
		g_pScreen = a_screen;
	}

	m_pCurrentLevel->Draw(m_pWorldSurface, Vector2(), m_cameraPosition);
	
	Entities->Draw(m_pWorldSurface, m_origin, m_pCurrentLevel, m_cameraPosition);
	m_pWorldSurface->CopyPartTo(a_screen, 0, 0, max(m_cameraPosition.x, 0), max(m_cameraPosition.y, 0), SCRWIDTH, SCRHEIGHT);
	if(Player::g_bStarActive)
	{
		Tmpl8::Pixel* sBuffer = a_screen->GetBuffer();
		for(int y = 0; y < SCRHEIGHT; y+=2)
			{
			for(int x = 0; x < SCRWIDTH; x+=2)
			{
				int i = y * SCRWIDTH + x;

				RgbColor rgb;
				rgb.r = (sBuffer[i] & 0xFF0000) >> 16;
				rgb.g = (sBuffer[i] & 0xFF00) >> 8;
				rgb.b = sBuffer[i] & 0xFF;
				HsvColor hsv = RgbToHsv(rgb);
				hsv.h += 100 * flashCounter;
				rgb = HsvToRgb(hsv);
				sBuffer[i] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
				sBuffer[i+1] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
				sBuffer[i+SCRWIDTH] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
				sBuffer[i+SCRWIDTH+1] = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
			}
		}

	}
	m_pUserInterface->Draw(a_screen);
}


void World::ChangeLevel(std::string a_level, Player* a_pPlayer, Vector2 a_targetPosition)
{
	Entities->ClearLevel(GetCurrentLevel());
	instance->m_pCurrentLevel = instance->m_pLevels[a_level];
	instance->m_sCurrentLevel = a_level;
	Entities->SetCurrentLevel(instance->m_pCurrentLevel);
	Triggers->m_pCurrentLevel = instance->m_pCurrentLevel;
	Tiles->m_sCurrentLevel = a_level;
	
	instance->m_oldCameraPosition = Vector2();

	GetCurrentLevel()->Init();
	Entities->SpawnPlayer(GetCurrentLevel(), a_pPlayer, a_targetPosition);
	instance->m_cameraPosition.x = Entities->GetPlayer()->GetPosition().x + Entities->GetPlayer()->GetSprite()->m_textureRect.width - SCRWIDTH/2;
	instance->m_cameraPosition.y = Entities->GetPlayer()->GetPosition().y + Entities->GetPlayer()->GetSprite()->m_textureRect.height - SCRHEIGHT/2;
}


void World::SetState(WorldState a_state)
{
}


void World::TestCollisions()
{	
	for(std::vector<Entity*>::iterator iter = Entities->GetEntities(GetCurrentLevel())->begin(); iter != Entities->GetEntities(GetCurrentLevel())->end(); iter++)
	{	
		Entity* ent = *iter;
	
		if(ent->m_type == STATIC || ent->m_type == EFFECT || ent->m_type == TRIGGER)
			continue; 

		if( ent->GetPosition().x + 96 < m_cameraPosition.x ||
			ent->GetPosition().x - 96 > m_cameraPosition.x + SCRWIDTH	)
		{
			continue;
		}

		if(ent->m_bUsesGravity && !ent->m_bIsOnFloor)
		{
			ent->m_velocity.y += FRAMES_PER_SECOND * 60;
			if(ent->m_velocity.y > 24)
			{
				ent->m_velocity.y = 24;
			}
		}
		ent->m_bIsOnFloor = false;
		
		CollisionRectangle entityRectangle = CollisionRectangle(
				ent->GetPosition().x + ent->m_velocity.x + ent->m_collisionRect.x,
				ent->GetPosition().y + ent->m_velocity.y + ent->m_collisionRect.y,
				ent->m_collisionRect.width,
				ent->m_collisionRect.height);

		CollisionRectangle gravityRect;
		
		if(ent->m_bUsesGravity)
		{
			gravityRect = CollisionRectangle(
				ent->GetPosition().x + ent->m_collisionRect.x,
				ent->GetPosition().y + ent->m_collisionRect.y + ent->m_collisionRect.height,
				ent->m_collisionRect.width,
				1);
		}
		
		if(ent->m_bCollidesWithTiles)
		{
			Vector2 displacement(0,0);
			CollisionRectangle tilesToCollideWith = GetTilesToCollideWith(entityRectangle);

			int lastTile = (tilesToCollideWith.y + tilesToCollideWith.height) * (GetCurrentLevel()->m_levelSize.x  / TILE_WIDTH) + (tilesToCollideWith.x + tilesToCollideWith.width);
			for(int y = tilesToCollideWith.y; y < tilesToCollideWith.y + tilesToCollideWith.height; y++)
			{
				for(int x = tilesToCollideWith.x; x < tilesToCollideWith.x + tilesToCollideWith.width; x++)
				{
					if(y > 64)
					{
						break;
					}
					int i = y * (GetCurrentLevel()->m_levelSize.x  / TILE_WIDTH) + x;
					CollisionType colType = Tiles->GetTileSurface(GetCurrentLevel()->m_tiles[i]->m_iTileId)->m_collisionType;
					if(colType == NONE || (colType == BOOMERANGABLE && ent->m_type == PLAYER_WEAPON))
						continue;

					CollisionRectangle tileRectangle = CollisionRectangle(GetCurrentLevel()->m_tiles[i]->m_collisionRectangle);
					CollisionInfo info = entityRectangle.Intersects(tileRectangle);
					CollisionInfo gravInfo = gravityRect.Intersects(tileRectangle);

					if(gravInfo.isColliding)
					{
						ent->Land();
					}

					if(info.isColliding)
					{
						if(abs(info.depth.x) < abs(info.depth.y))
						{
							if(info.depth.x > 0 && ent->m_velocity.x < 0 || info.depth.x < 0 && ent->m_velocity.x > 0)
							{
								ent->Move(info.depth.x + ent->m_velocity.x, 0);
								ent->m_velocity.x = 0;
								entityRectangle.x  += info.depth.x + ent->m_velocity.x;
								if( Bounceable* bounceableEntity = dynamic_cast<Bounceable*>(*iter) )
								{
									bounceableEntity->Bounce();
								}
							}
						}
						else
						{						
							if(info.depth.y > 0 && ent->m_velocity.y < 0 || info.depth.y < 0 && ent->m_velocity.y > 0)
							{
								ent->Move(0,info.depth.y + ent->m_velocity.y);
								ent->m_velocity.y = 0;
								entityRectangle.y  += info.depth.y + ent->m_velocity.y;
							}
						}
					}
				}
			}
		}
		
		for(std::vector<Entity*>::iterator otherIter = Entities->GetEntities(GetCurrentLevel())->begin(); otherIter != Entities->GetEntities(GetCurrentLevel())->end(); otherIter++)
		{
			Entity* otherEnt = *otherIter;
			if(ent == *otherIter)
				continue;

			if(otherEnt->m_type == EFFECT)
				continue; 

			if( otherEnt->GetPosition().x + 96 < m_cameraPosition.x ||
			otherEnt->GetPosition().x - 96 > m_cameraPosition.x + SCRWIDTH)
			{
				continue;
			}

			if(Vector2(ent->GetPosition() + Vector2(ent->m_collisionRect.x, ent->m_collisionRect.y)).DistanceSquared(otherEnt->GetPosition()) > 10000
				&& otherEnt->m_type != TRIGGER)
				continue;

			CollisionRectangle otherEntityRectangle = CollisionRectangle(
				otherEnt->GetPosition().x + otherEnt->m_collisionRect.x,
				otherEnt->GetPosition().y + otherEnt->m_collisionRect.y,
				otherEnt->m_collisionRect.width,
				otherEnt->m_collisionRect.height);

			CollisionInfo info = entityRectangle.Intersects(otherEntityRectangle);

			if(info.isColliding)
			{
				ent->Hit(*otherIter, info);
			}
			if(otherEnt->m_type == STATIC && ent->m_bUsesGravity)
			{
				CollisionInfo gravInfo = gravityRect.Intersects(otherEntityRectangle);
				if(gravInfo.isColliding)
				{
					ent->Land();
				}
			}
		}
	}
}


CollisionRectangle World::GetTilesToCollideWith(CollisionRectangle a_ownRectangle)
{
	int rectWidth = a_ownRectangle.width;
	int widthInTiles = 0;
	while(rectWidth > 0)
	{
		rectWidth -= TILE_WIDTH;
		widthInTiles++;
	}

	int rectHeight = a_ownRectangle.height;
	int heightInTiles = 0;
	while(rectHeight > 0)
	{
		rectHeight -= TILE_HEIGHT;
		heightInTiles++;
	}

	CollisionRectangle ownRectInTiles(a_ownRectangle.x / TILE_WIDTH, a_ownRectangle.y / TILE_WIDTH, widthInTiles , heightInTiles);
	CollisionRectangle bounds(ownRectInTiles.x - 2, ownRectInTiles.y - 2, 5, 5);
	if(bounds.x + bounds.width > GetCurrentLevel()->m_levelSize.x / TILE_WIDTH)
	{
		bounds.width = (GetCurrentLevel()->m_levelSize.x / TILE_WIDTH) - bounds.x;
	}
	if(bounds.y + bounds.height > GetCurrentLevel()->m_levelSize.y / TILE_HEIGHT)
	{
		bounds.height = (GetCurrentLevel()->m_levelSize.y / TILE_HEIGHT) - bounds.y;
	}

	bounds.x = max(bounds.x, 0);
	bounds.y = max(bounds.y, 0);

	return bounds;
}


void World::PlacePlayer()
{
	//Tiles->SetLevel(41);
	Entities->GetPlayer()->SetPosition(Vector2(100,300));
}



void World::EndGame()
{

}