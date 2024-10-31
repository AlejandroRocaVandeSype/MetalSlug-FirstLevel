#include "pch.h"
#include "Enemy.h"


Enemy::Enemy(const GameObject::Type& id, const Point2f& startPos, int health, const Point2f& speed,
	bool comingFromRight, const Point2f& acceleration, SoundManager* sounds)
	: GameObject(id, startPos, speed, comingFromRight, sounds, acceleration)
	, m_Health { health }
	, m_ActionState { Enemy::ActionState::state1 }
	, m_PreviousAction { Enemy::ActionState::state1 }
	, m_MaxTimeDeath { 2.f }
	, m_SecondsDeath { }
	, m_SecondsWaiting { }
	, m_IsFacingRight { }
{

}


void Enemy::ChangeSprite(const int cols, const int rows, const int frames, const float frameSec, const float width,
	const float height, const float spriteSheetTop)
{
	m_pBottomSprite->UpdateValues(cols, rows, frames, frameSec, width, height, spriteSheetTop);
	m_pBottomSprite->ResetSprite();

	m_BotSpriteChanged = false;
}

// Check into which direction is the soldier loocking
void Enemy::CheckFacingRight(const Rectf& avatarShape)
{
	if (avatarShape.left + avatarShape.width >
		m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width)
	{
		m_IsFacingRight = true;
	}
	else
	{
		m_IsFacingRight = false;
	}

}

// Check if soldier changed action in order to reset sprite correctly
void Enemy::CheckPreviousAction()
{
	if (m_ActionState != m_PreviousAction)
	{
		m_PreviousAction = m_ActionState;
		m_BotSpriteChanged = true;
	}
}


void  Enemy::KillSoldier(float elapsedSec)
{
	m_pBottomSprite->Update(elapsedSec, false);

	if (m_pBottomSprite->GetAnimationFinish())
	{
		m_SecondsDeath += elapsedSec;

		if (m_SecondsDeath >= m_MaxTimeDeath)
		{
			m_IsDead = true;
			m_pBottomSprite->ResetSprite();
			m_IsActive = false;
			m_SecondsDeath = 0.f;
		}
	}


}

// Randomly chooses the death sound for the soldiers
std::string Enemy::GetDeathSound()
{
	int deathEffect = rand() % 3;

	std::string filename{};

	switch (deathEffect)
	{
	case 0:
		filename = "Resources/Sounds/Soldierdies1.wav";
		break;

	case 1:
		filename = "Resources/Sounds/Soldierdies2.wav";
		break;

	case 2:
		filename = "Resources/Sounds/Soldierdies3.mp3";
		break;
	}

	return filename;
}
