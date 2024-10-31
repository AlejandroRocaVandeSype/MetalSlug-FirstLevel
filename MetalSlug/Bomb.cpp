#include "pch.h"
#include "Bomb.h"


Bomb::Bomb()
	: Projectile( GameObject::Type::bomb, Point2f{}, Point2f{ 0.f, 20.f }, nullptr, Point2f{ 0.f, -600.f } )
	, m_FallingAcc{ -600.f }
{
	Initialize();
}


Bomb::~Bomb()
{
	delete m_pTopSprite;
	delete m_pBottomSprite;
}


void Bomb::Initialize()
{
	m_Acceleration.y = m_FallingAcc;

	// Bomb sprite
	m_pTopSprite = new Sprite("Resources/sprites/enemies/BombHeli.png");
	m_pTopSprite->UpdateValues(7, 1, 7, 15.f, 25.f, 26.f, 26.f, 2.7f);

	// Explosion sprite
	m_pBottomSprite = new Sprite("Resources/sprites/enemies/Explosion.png");
	m_pBottomSprite->UpdateValues(5, 1, 5, 15.f, 35.2f, 64.f, 64.f, 2.7f);

}


void Bomb::Update(float elapsedSec, const Rectf& actorShape)
{
	
	if (!m_IsHit)
	{
		// Bomb Update
		if (!m_IsInitialized)
		{
			SetStartPos(actorShape);
		}

		Move(elapsedSec);
		m_pTopSprite->Update(elapsedSec, true);

	}
	else
	{
		// Explosion Update
		m_pBottomSprite->Update(elapsedSec, false);

		if (m_pBottomSprite->GetAnimationFinish())
		{
			m_pBottomSprite->ResetSprite();
			DesactivateProjectile();

			m_Velocity.y = m_Speed.y;
			m_Acceleration.y = m_FallingAcc;
		}
	}
	
}

void Bomb::SetStartPos(const Rectf& helicopter)
{
	Point2f startPos{ };

	startPos.y = helicopter.bottom + m_pTopSprite->GetFrameWidth() / 2;
	startPos.x = helicopter.left + helicopter.width / 3;

	// Position from the bullet start moving
	m_Position = startPos;
	m_IsInitialized = true;

	m_pTopSprite->SetLeftDstRect(startPos.x);
	m_pTopSprite->SetBottomDstRect(startPos.y);
}

void Bomb::Move(float elapsedSec)
{
	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;
	m_pTopSprite->SetBottomDstRect(m_Position.y);

}

void Bomb::CheckGameState()
{

}

Sprite* Bomb::GetBotSprite() const
{
	return m_pBottomSprite;
}

Sprite* Bomb::GetTopSprite() const
{
	return m_pTopSprite;
}

