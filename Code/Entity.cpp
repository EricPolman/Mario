#include "Entity.h"
#include "EntityManager.h"


Entity::Entity(std::string a_tilesheetName,float a_fX, float a_fY, Level* a_room, int a_iWidth, int a_iHeight)
{
	m_pSprite = new AnimatedSprite();
	m_pSprite->LoadSpritesheet(a_tilesheetName, a_iWidth, a_iHeight, 1, 1);

	m_position.x = a_fX;
	m_position.y = a_fY;
	m_state = ALIVE;
	m_type = ENTITY;
	m_bIsVisible = true;
	m_bCollidesWithTiles = false;
	m_bUsesGravity = false;
	m_bIsOnFloor = false;

	m_collisionRect = CollisionRectangle(0,0, m_pSprite->GetWidth(), m_pSprite->GetHeight());
	if(a_room != 0)
	{
		Entities->Add(this, a_room);
	}
	else
	{
		Entities->Add(this);
	}
	m_zIndex = 1;
}

Entity::Entity(const Entity& a_other)
{
	m_pSprite = new AnimatedSprite(*a_other.GetSprite());
	m_position = a_other.GetPosition();
	m_state = ALIVE;
	m_type = a_other.m_type;
	m_bIsVisible = a_other.m_bIsVisible;
	m_bCollidesWithTiles = a_other.m_bCollidesWithTiles;
	m_bUsesGravity = a_other.m_bUsesGravity;
	m_bIsOnFloor = a_other.m_bIsOnFloor;

	m_collisionRect = a_other.m_collisionRect;
	
	m_zIndex = a_other.m_zIndex;

	Entities->Add(this);
}

Entity::~Entity(void)
{
	Entities->Remove(this);
}


void Entity::Update(float a_fDeltaTime)
{
	m_pSprite->Update(a_fDeltaTime);
}


void Entity::Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther)
{

}


void Entity::Draw(Tmpl8::Surface* a_surface, Vector2 a_origin)
{
	if(m_bIsVisible)
	{
		m_pSprite->Draw(a_surface, a_origin + m_position);//CopyTo(a_surface, a_origin.x + m_position.x, a_origin.y + m_position.y, true);
	}
	/*a_surface->Plot(m_position.x, m_position.y, 0xFFFE00FE);
	a_surface->Plot(m_position.x+1, m_position.y, 0xFFFE00FE);
	a_surface->Plot(m_position.x+1, m_position.y+1, 0xFFFE00FE);
	a_surface->Plot(m_position.x, m_position.y+1, 0xFFFE00FE);*/
}


void Entity::Move(float a_fX, float a_fY)
{
	m_position.x += a_fX;
	m_position.y += a_fY;
}


void Entity::Move(Vector2 a_translation)
{
	m_position.x += a_translation.x;
	m_position.y += a_translation.y;
}
