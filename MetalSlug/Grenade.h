#pragma once
#include "Projectile.h"

class Soldier;
class Avatar;
class SoundEffect;
class Grenade final : public Projectile
{

public:

	Grenade(SoundManager* sounds);
	Grenade(const Grenade& grenade) = delete;
	Grenade(Grenade&& grenade) = delete;
	Grenade& operator=(const Grenade& grenade) = delete;
	Grenade& operator=(Grenade&& grenade) = delete;
	~Grenade();

	virtual void CheckGameState() override;
	virtual void Draw() const override; 

	void Update(float elapsedSec, Avatar* avatar, const Soldier* soldier, const std::vector<Point2f>& vertices);
	void SetStartPos(const Point2f& soldierPos);


private:

	float m_Angle;				// Rotation angle for the grenade 
	bool m_IsMovingRight;

	// Sound
	SoundEffect* m_pExplosionSound;

	virtual void Initialize() override;

	void CheckHitLevel(const std::vector<Point2f>& vertices);
	void CheckHitAvatar(Avatar* avatar);

	virtual void Move(float elapsedSec) override;
	

};

