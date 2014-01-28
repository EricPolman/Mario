#pragma once
#include "Entity.h"
#include "MysteryCube.h"

enum PlayerState {PLAYER_ATTACKING, PLAYER_WALKING, PLAYER_IDLE, PLAYER_GROWING, PLAYER_LOWERING_FLAG, PLAYER_WALK_TO_CASTLE, PLAYER_ENTERING_CASTLE, PLAYER_IN_TUBE, PLAYER_CLIMBING };

class Player : public Entity
{
public:
	Player(std::string a_tilesheetName, std::string a_level, float a_fX = 0, float a_fY = 0);
	virtual ~Player();
	
	virtual void Update(float a_fDeltaTime);
	void Land();
	
	void SetDirection(Direction a_direction){m_direction = a_direction;}
	inline Direction GetDirection() const{return m_direction;}

	bool GetIsLarge() const{return m_bIsLarge;}
	bool GetHasFireFlower() const{return m_bHasFireFlower;}

	void Hit(Entity* a_pOther, const CollisionInfo& a_info, bool a_bCalledFromOther = false);
	void Spawn();

	void Pickup(MysteryReward a_reward);

	void PullDownFlag(Entity* a_pFlagPole);
	void EnterCastle();
	void EnterTube(Direction a_direction, Vector2 a_targetPosition, std::string a_sTargetLevel);

	void Hurt();
	void Climb();

	Direction m_tubeDirection;
	Vector2 m_tubeTargetPosition;
	std::string m_tubeTargetLevel;
	float m_fTubeDistanceTraveled;

	bool m_bClearedLevel, m_bDied;
	static bool g_bStarActive;
	void Die();
private:
	float m_fStarActiveTimer;
	Direction m_direction;
	void ChangeDirection(Direction a_direction);
	bool m_bDirectionChanged;
	PlayerState m_playerState;
	
	float m_fRespawnTimer;

	float m_fJumpBoostTimer;
	Entity* m_flagPole;
	
	bool m_bIsLarge;
	bool m_bHasFireFlower;
	
	float m_fInvulnerableTimer;
	bool m_bIsVulnerable;
	float m_fClimbTimer;
};
