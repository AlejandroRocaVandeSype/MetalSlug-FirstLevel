#include "pch.h"
#include "BossBullet.h"
#include "utils.h"
#include <iostream>


BossBullet::BossBullet(bool airBullet, const Point2f& startPos, SoundManager* sounds)
	:Projectile(GameObject::Type::bossBullet, startPos, Point2f{ 350.f , 1500.f}, sounds, Point2f{0.f, g_Gravity})
	, m_BulletType{ BulletType::airBullet }
	, m_InitialVelY { 500.f }
	, m_Velocity { 350.f, m_InitialVelY }
{
	if (airBullet)
	{
		m_BulletType = BulletType::airBullet;
	}
	else
	{
		m_BulletType = BulletType::groundBullet;
	}

	Initialize();
}


BossBullet::~BossBullet()
{
	delete m_pTopSprite;
}


void BossBullet::Initialize()
{
	m_pTopSprite = new Sprite("Resources/sprites/enemies/Boss/projectiles.png");

	if (m_BulletType == BulletType::airBullet)
	{
		m_pTopSprite->UpdateValues(3, 1, 3, 8.f, 10.f, 10.f, 20.f);     // Flying bullet
	}
	else
	{
		m_pTopSprite->UpdateValues(4, 1, 4, 10.f, 80.f, 26.f, 50.f);   // Ground Bullet
	}

	// Start Position
	m_pTopSprite->SetLeftDstRect(m_StartPosition.x);
	m_pTopSprite->SetBottomDstRect(m_StartPosition.y);

}


void BossBullet::CheckGameState()
{

}

void BossBullet::Update(float elapsedSec)
{
	m_pTopSprite->Update(elapsedSec, true);

	Move(elapsedSec);
}

void BossBullet::Draw() const
{
	if (m_BulletType == BulletType::airBullet)
	{
		if (!m_IsHit)
		{
			m_pTopSprite->Draw();
		}
	}
	else
	{
		// Always draw the ground bullet
		m_pTopSprite->Draw();
	}
	

}


void BossBullet::Move(float elapsedSec)
{
	m_Seconds += elapsedSec;

	if (m_BulletType == BulletType::airBullet)
	{
		
		if (m_Seconds >= 2.5f)
		{
			m_IsActive = false;
			m_Seconds = 0;
			m_IsHit = false;

			m_Velocity.y = float((rand() % 500) + 300);

			m_pTopSprite->SetLeftDstRect(m_StartPosition.x);
			m_pTopSprite->SetBottomDstRect(m_StartPosition.y);

		}
		else
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
			m_pTopSprite->SetLeftDstRect(m_pTopSprite->GetDstRect().left - (m_Velocity.x * elapsedSec));
			m_pTopSprite->SetBottomDstRect(m_pTopSprite->GetDstRect().bottom + (m_Velocity.y * elapsedSec));
		}
	}
	else
	{
		// Ground Bullet
		if (m_Seconds >= 2.5f)
		{
			m_IsActive = false;
			m_Seconds = 0;
			m_IsHit = false;
			m_pTopSprite->SetLeftDstRect(m_StartPosition.x);
		}
		else
		{

			m_pTopSprite->SetLeftDstRect(m_pTopSprite->GetDstRect().left - (m_Speed.x * elapsedSec));
		}
	}
}

const BossBullet::BulletType& BossBullet::GetBulletType() const
{
	return m_BulletType;
}