#pragma once
#include "Enemy.h"

class Grenade;
class Soldier final : public Enemy
{

public:

	Soldier(const Point2f& startPos, bool comingFromRight, SoundManager* sounds);
	~Soldier();
	Soldier(const Soldier& soldier) = delete;
	Soldier(Soldier&& soldier) = delete;
	Soldier& operator=(const Soldier& soldier) = delete;
	Soldier& operator=(Soldier&& soldier) = delete;


	virtual void Draw() const override;
	virtual void Hit() override;
	virtual void CheckGameState() override;

	virtual void Update(float elapsedSec, Avatar* avatar, const Level* level) override;


	bool GetFacingRight() const;


private:

	const float m_RunSpeed;
	const float m_RunAwayDist;
	const float m_ThrowGrenadeDist;
	bool m_IsReset;							// Animation has been reset already

	// TIMERS
	// Time the soldier runs Away from avatar
	const float m_MaxTimeRunAway;
	float m_Seconds;

	// Max Time soldier waits until attack again 
	const float m_MaxTimeWait;

	Grenade* m_pGrenade;

	// Death sound
	SoundEffect* m_pDeathSound;

	// PRIVATE FUNCTIONS

	virtual void Initialize() override;
	void Move(float elapsedSec);

	void ActivateGrenade();

	// Action States (AI) for soldier
	void DoWalkingState(float elapsedSec, const Rectf& avatarShape);
	void DoAttackState(float elapsedSec, const Rectf& avatarShape);
	void DoRunAwayState(float elapsedSec, const Rectf& avatarShape);
	void DoWaitingState(float elapsedSec, const Rectf& avatarShape);



	void UpdateFrames(float elapsedSec);

	void CheckDistanceAvatar(const Rectf& avatarShape);
	

};

