#include "pch.h"
#include "Shield.h"
#include "Level.h"

Shield::Shield(const Point2f& startPos)
	:GameObject(GameObject::Type::shield, startPos, Point2f{}, false, nullptr, Point2f{-40.f, g_Gravity})
	, m_ImpactSpeed { 40.f, 450.f }
	, m_Impact { true }
{
	Initialize();
}


Shield::~Shield()
{
	delete m_pBottomSprite;
}


void Shield::Initialize()
{

	m_pBottomSprite = new Sprite("Resources/sprites/enemies/flyingShield.png");
	m_pBottomSprite->UpdateValues(10, 1, 10, 12.f, 34.f, 38.f, 38.f);  // Flying shield
	m_pBottomSprite->SetLeftDstRect(m_StartPosition.x);
	m_pBottomSprite->SetBottomDstRect(m_StartPosition.y);
	m_pBottomSprite->UpdateLeftSrcRect();

}

void Shield::Draw() const
{
	m_pBottomSprite->Draw();
}

void Shield::Update(float elapsedSec, const Level* level)
{

	m_pBottomSprite->Update(elapsedSec, false);

	// Shield destroyed --> Start flying away
	if (m_Impact)
	{
		m_Velocity.y = m_ImpactSpeed.y;
		m_Velocity.x = m_ImpactSpeed.x;
		m_Impact = false;  // Only once
	}
	m_Velocity.x += m_ImpactSpeed.x;

	level->HandleCollision(m_pBottomSprite->GetDstRect(), m_Velocity);
	Falling(elapsedSec, level);

}




void Shield::Hit()
{

}

void Shield::CheckGameState()
{

}