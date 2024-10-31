#pragma once
#include "Enemy.h"

class BossBullet;
class Boss final : public Enemy
{
public:

	enum class State {

		sleeping,
		starting,
		firingUp,
		waiting,
		chargingShot,
		firingBot,
		dying

	};


	Boss(const Point2f& startPos, bool comingFromRight, SoundManager* sounds);
	~Boss();
	Boss(const Boss& soldier) = delete;
	Boss(Boss&& soldier) = delete;
	Boss& operator=(const Boss& soldier) = delete;
	Boss& operator=(Boss&& soldier) = delete;


	virtual void Draw() const override;
	virtual void Hit() override;
	virtual void CheckGameState() override;

	virtual void Update(float elapsedSec, Avatar* avatar, const Level* level) override;

	void CheckHitBullet(Avatar* avatar);
	Point2f GetStartPos() const;

private:

	BossBullet* m_pWaterBullet;
	BossBullet* m_pAirBullets;
	std::vector <BossBullet*> m_pBullets;

	State m_FightState;
	State m_PreviousState;

	SoundEffect* m_pExplosion;
	SoundEffect* m_pWaterShoot;
	SoundEffect* m_pAirShoot;
	SoundEffect* m_pMoving;

	float m_Seconds;
	int m_NrOfShots;
	int m_NrOfChargings;
	int m_NextFrame;

	const Point2f m_BulletAirSpeed;
	const float m_CloathSpeed;

	virtual void Initialize() override;

	
	void StartFight(float elapsedSec);
	void FiringUp(float elapsedSec);
	void FiringBot(float elapsedSec);
	void ChargingShot(float elapsedSec);
	void Waiting(float elapsedSec);
	void Dying(float elapsedSec);
	void DyingAnimation(float elapsedSec);
	void SelectNextShotFrame();
};

