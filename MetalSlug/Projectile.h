#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{

public:

	Projectile(const GameObject::Type& type, const Point2f& startPos, const Point2f& speed, 
		SoundManager* sounds, const Point2f& acceleration);
	virtual ~Projectile() = default;
	Projectile(const Projectile & projectile) = delete;
	Projectile(Projectile && projectile) = delete;
	Projectile& operator=(const Projectile & projectile) = delete;
	Projectile& operator=(Projectile && projectile) = delete;

	virtual void CheckGameState() = 0;

	virtual void Draw() const override;

	void Hit();
	void DesactivateProjectile();

	bool GetIsHit();

protected:

	Point2f m_Position;

	bool m_IsInitialized;
	bool m_IsHit;

	const float m_MaxSeconds;     // Time the projectile is active if it doesn't collide 
	float m_Seconds;			  // Current Seconds


	virtual void Move(float elapsedSec) = 0;
	virtual void Initialize() = 0;

};

