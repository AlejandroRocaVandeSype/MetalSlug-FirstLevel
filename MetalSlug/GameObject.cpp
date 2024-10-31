#include "pch.h"
#include "GameObject.h"
#include "Level.h"
#include "SoundManager.h"

GameObject::GameStage GameObject::m_GameState = GameStage::moving;
bool GameObject::m_IsFirstHeliFightStart = false;
bool GameObject::m_IsSecondHeliFightStart = false;
bool GameObject::m_IsBossFightStart = false;
bool GameObject::m_IsStageChanged = false;

GameObject::GameObject( const GameObject::Type& id, const Point2f& startPos, const Point2f& speed,  bool comingFromRight, 
	SoundManager* sounds, const Point2f& acceleration)
	: m_Id{ id }
	, m_StartPosition { startPos }
	, m_pTopSprite{ }
	, m_pBottomSprite{ }
	, m_IsActive{ false }
	, m_IsDead { false }
	, m_IsDying { false }
	, m_BotSpriteChanged{ false }
	, m_TopSpriteChanged{ false }
	, m_Velocity{ }
	, m_Acceleration{ acceleration }
	, m_Speed { speed }
	, m_IsComingFromRight { comingFromRight }
	, m_pSoundManager { sounds }
	, m_Points{ }
{
	SetPointsGameObject(id);
}





// Gravity for the gameObjects
void GameObject::Falling(float elapsedSec, const Level* level)
{
	if (!level->IsOnGround(m_pBottomSprite->GetDstRect(), m_Velocity))
	{
		m_Velocity.y += m_Acceleration.y * elapsedSec;
	}

	m_pBottomSprite->SetBottomDstRect(m_pBottomSprite->GetDstRect().bottom + m_Velocity.y * elapsedSec);
}

void GameObject::SetIsActive(bool active)
{
	m_IsActive = active;
}

void GameObject::SetGameStageChanged(bool stageChanged)
{
	m_IsStageChanged = stageChanged;
}

// How many points each Game Object gives 
void GameObject::SetPointsGameObject(const GameObject::Type& id)
{
	switch (id)
	{
	case GameObject::Type::helicopter:
	case GameObject::Type::soldier:
	case GameObject::Type::soldierShield:
	case GameObject::Type::boss:
		m_Points = 100;
		break;
	case GameObject::Type::item:
		m_Points = 5000;
		break;
	};
}


Rectf GameObject::GetTopShape() const
{
	return m_pTopSprite->GetDstRect();
}

Rectf GameObject::GetBotShape() const
{
	return m_pBottomSprite->GetDstRect();
}

Sprite* GameObject::GetTopSprite() const
{
	return m_pTopSprite;
}

Sprite* GameObject::GetBottomSprite() const
{
	return m_pBottomSprite;
}

GameObject::GameStage& GameObject::GetGameStage() const
{
	return m_GameState;
}

bool GameObject::GetGameStageChanged() const
{
	return m_IsStageChanged;
}

GameObject::Type GameObject::GetType() const
{
	return m_Id;
}

bool GameObject::GetIsDying() const
{
	return m_IsDying;
}

bool GameObject::GetIsComingFromRight() const
{
	return m_IsComingFromRight;
}

bool GameObject::GetIsActive() const
{
	return m_IsActive;
}

bool GameObject::GetIsDead() const
{
	return m_IsDead;
}

const unsigned int GameObject::GetPointsGameObject() const
{
	return m_Points;
}