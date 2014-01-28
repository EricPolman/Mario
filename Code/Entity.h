#pragma once
#include "includes.h"
#include "AnimatedSprite.h"
#include "Level.h"

enum EntityType{ENTITY,PLAYER,ENEMY, SWORD_SLASH, DESTROYABLE, EFFECT, PLAYER_WEAPON, STATIC, PICKUP, BOSS, TRIGGER};
enum EntityState{ALIVE, DEAD, REMOVED};

class Entity
{
public:
	Entity(std::string a_tilesheetName, float a_fX = 0, float a_fY = 0, Level* a_room = 0, int a_iWidth = 32, int a_iHeight = 32); 
	virtual ~Entity(void);
	Entity(const Entity&);
	
	EntityState m_state;
	EntityType m_type;

	bool m_bUsesGravity;
	bool m_bIsOnFloor;

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Tmpl8::Surface* a_screen, Vector2 a_origin);
	virtual void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	virtual void Move(Vector2 a_translation);
	virtual void Move(float a_fX, float a_fY);
	virtual void Hurt(){}

	virtual void Land(){m_bIsOnFloor = true;}

	void SetPosition(Vector2 a_position){m_position = a_position;}
	inline Vector2 GetPosition()const{return m_position;}
	
	void SetSprite(AnimatedSprite* a_pSprite){m_pSprite = a_pSprite;}
	inline AnimatedSprite* GetSprite() const{return m_pSprite;}
	
	Vector2 m_velocity;

	bool m_bIsVisible;
	CollisionRectangle m_collisionRect;
	bool m_bCollidesWithTiles;

	int m_zIndex;
protected:
	Vector2 m_position;
	AnimatedSprite* m_pSprite;
};

