#pragma once

#include "Projectile.h"

class BossBullet final : public Projectile
{

public:

	enum class BulletType {

		groundBullet,
		airBullet

	};


	BossBullet(bool airBullet, const Point2f& startPos, SoundManager* sounds);
	BossBullet(const BossBullet& grenade) = delete;
	BossBullet(BossBullet&& grenade) = delete;
	BossBullet& operator=(const BossBullet& grenade) = delete;
	BossBullet& operator=(BossBullet&& grenade) = delete;
	~BossBullet();

	virtual void CheckGameState() override;
	virtual void Draw() const override;

	void Update(float elapsedSec);

	const BulletType& GetBulletType() const;

private:

	BulletType m_BulletType;

	const float m_InitialVelY;
	Point2f m_Velocity;
	

	virtual void Initialize() override;
	virtual void Move(float elapsedSec) override;

};

