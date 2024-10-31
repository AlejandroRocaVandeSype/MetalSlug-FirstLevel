
// Bombs from the helicopter
#pragma once
#include "Projectile.h"


class Bomb final : public Projectile
{

public:
	Bomb( );
	~Bomb();
	Bomb( const Bomb& bomb ) = delete;
	Bomb(Bomb&& bomb) = delete;
	Bomb& operator=( const Bomb& bomb ) = delete;
	Bomb& operator=( Bomb&& bomb ) = delete;

	virtual void CheckGameState() override;
	

	void Update(float elapsedSec, const Rectf& actorShape);

	Sprite* GetBotSprite() const;
	Sprite* GetTopSprite() const;

private:

	const float m_FallingAcc;

	virtual void Initialize() override;
	void SetStartPos(const Rectf& helicopter);
	virtual void Move(float elapsedSec) override;

};

