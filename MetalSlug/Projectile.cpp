#include "pch.h"
#include "Projectile.h"
#include "SoundEffect.h"
#include "SoundManager.h"



Projectile::Projectile(const GameObject::Type& type, const Point2f& startPos, const Point2f& speed, 
	SoundManager* sounds, const Point2f& acceleration)
	:GameObject(type, startPos, speed, false, sounds, acceleration)
	, m_MaxSeconds { 0.7f }
	, m_Seconds { }
	, m_IsHit{ false }
	, m_IsInitialized { false }
{
	
}


void Projectile::Draw() const
{
	if (m_IsHit)
	{
		// Draw explosion
		if (m_pBottomSprite != nullptr)
		{
			m_pBottomSprite->Draw();
		}
	}
	else
	{
		// Draw projectile 
		m_pTopSprite->Draw();

	}

}

// Projectile hit with something
void Projectile::Hit()
{
	m_IsHit = true;

	if (m_pBottomSprite != nullptr)
	{
		// Save position of the collision
		m_pBottomSprite->SetLeftDstRect(m_Position.x);
		m_pBottomSprite->SetBottomDstRect(m_Position.y);

	}

	
}


void Projectile::DesactivateProjectile()
{
	m_IsActive = false;
	m_IsInitialized = false;
	m_Seconds = 0.f;
	m_Velocity = Vector2f{ 0.f, 0.f };
	m_IsHit = false;
}

bool Projectile::GetIsHit()
{
	return m_IsHit;
}