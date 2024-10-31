#pragma once
#include "Enemy.h"

class Shield;
class Avatar;
class SoldierShield final : public Enemy
{
public:

	SoldierShield(const Point2f& startPos, bool comingFromRight, SoundManager* sounds);
	~SoldierShield();
	SoldierShield(const SoldierShield & soldierShield) = delete;
	SoldierShield(SoldierShield && soldierShield) = delete;
	SoldierShield& operator=(const SoldierShield & soldierShield) = delete;
	SoldierShield& operator=(SoldierShield && soldierShield) = delete;


	virtual void Draw() const override;
	virtual void Hit() override;
	virtual void CheckGameState() override;

	virtual void Update(float elapsedSec, Avatar* avatar, const Level* level) override;


private:

	virtual void Initialize() override;
	
	
	// Action States (AI) for soldier with shield
	void DoRunningState( float elapsedSec, const Rectf& avatarShape);
	void DoAttackState( float elapsedSec, Avatar* avatar);
	void DoWaitState(float elapsedSec, Avatar* avatar);

	void StartAttackAnimation();
	void FinishAttackAnimation(Avatar* avatar);
	void SoldierDeathAnimation();
	void StartFlyingShieldAnimation();

	void CheckDistanceAvatar(const Rectf& avatarShape);
	void CheckAvatarHit(Avatar* avatar);

	void Move(float elapsedSec);
	void UpdateFrames(float elapsedSec);


	// Booleans
	bool m_IsShieldOn;				// To know if the soldier is carrying the shield or not
	bool m_IsAttackStarted;			
	bool m_IsAttacking;

	Shield* m_pShield;				// Flying shield

	const float m_RunSpeed;		

	const float m_AttackDist;

	// Time soldier waits until attack again  
	const float m_MaxTimeWait;

	// Health of the shield and the soldier
	int m_ShieldHealth;
	int m_SoldierHealth;


	// Sounds
	SoundEffect* m_pDeathSound;
};

