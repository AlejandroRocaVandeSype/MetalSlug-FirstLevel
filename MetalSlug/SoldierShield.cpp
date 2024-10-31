#include "pch.h"
#include "SoldierShield.h"
#include "Shield.h"
#include "Level.h"
#include "Avatar.h"
#include "SoundEffect.h"
#include <iostream>



SoldierShield::SoldierShield(const Point2f& startPos, bool comingFromRight, SoundManager* soundManager)
	:Enemy(GameObject::Type::soldierShield, startPos, 3, Point2f{ 150.f, 0.f },
		comingFromRight, Point2f{ 0.f, g_Gravity }, soundManager)   // Type, starting position, health, speed and accelerat
	, m_IsShieldOn { true }
	, m_pShield { }
	, m_MaxTimeWait { 0.8f }
	, m_RunSpeed { 200.f }
	, m_AttackDist { 60.f }
	, m_IsAttackStarted { false }
	, m_IsAttacking { false }
	, m_ShieldHealth { 3 }
	, m_SoldierHealth { 1 }
	, m_pDeathSound { soundManager->GetEffect( GetDeathSound() )}
{
	Initialize();

}


SoldierShield::~SoldierShield()
{
	delete m_pBottomSprite;

	if (m_pShield != nullptr)
	{
		delete m_pShield;
	}
}



void SoldierShield::Initialize()
{
	
	m_pBottomSprite = new Sprite("Resources/sprites/enemies/soldierShield.png");
	m_pBottomSprite->UpdateValues(12, 1, 12, 15.f, 35.f, 41.f, 100.f); // Soldier with shield running
	m_pBottomSprite->SetLeftDstRect(m_StartPosition.x);
	m_pBottomSprite->SetBottomDstRect(m_StartPosition.y);
	m_pBottomSprite->UpdateLeftSrcRect();

}
void SoldierShield::Draw() const
{
	if (!m_IsShieldOn)
	{
		// Draw shield flying away
		m_pShield->Draw();
	}

	
	if (m_IsFacingRight)
	{
		// Flip to the right
		glPushMatrix();

			m_pBottomSprite->FlipSprite();
			m_pBottomSprite->Draw();

		glPopMatrix();
	}
	else
	{
		m_pBottomSprite->Draw();
	}

}


void SoldierShield::Update(float elapsedSec, Avatar* avatar, const Level* level)
{
	CheckPreviousAction();

	if (!m_IsDying)
	{
		// Soldier is still alive
		switch (m_ActionState)
		{
		case Enemy::ActionState::state1:   // Run towards avatar
			DoRunningState( elapsedSec, avatar->GetBotShape() );
			break;
		case Enemy::ActionState::state2:  // Attack
			DoAttackState(elapsedSec, avatar);
			break;
		}
	}
	else
	{
		// Soldier is dead
		KillSoldier(elapsedSec);
	}


	if (!m_IsShieldOn)
	{
		// Shield flying away
		m_pShield->Update(elapsedSec, level);
	}
	
	UpdateFrames(elapsedSec);
	Falling(elapsedSec, level);
	Move(elapsedSec);
	level->HandleCollision(m_pBottomSprite->GetDstRect(), m_Velocity);
	
}

void SoldierShield::Move(float elapsedSec)
{
	m_pBottomSprite->SetLeftDstRect(m_pBottomSprite->GetDstRect().left + m_Velocity.x * elapsedSec);
	m_pBottomSprite->SetBottomDstRect(m_pBottomSprite->GetDstRect().bottom + m_Velocity.y * elapsedSec);

}

// Run towards the avatar
void SoldierShield::DoRunningState(float elapsedSec, const Rectf& avatarShape)
{
	// Check if we have to change the sprite
	if (m_BotSpriteChanged)  
	{
		if (m_IsShieldOn)
		{
			ChangeSprite(12, 1, 12, 15.f, 35.f, 41.f, 100.f); // Soldier with shield running;
		}
		else
		{
			ChangeSprite(12, 1, 12, 10.f, 34.f, 40.f, 368.f);  // Soldier without sield running
		}
		
	}

	CheckDistanceAvatar(avatarShape);
	CheckFacingRight(avatarShape);

	// Move in the direction of the avatar
	if (m_IsFacingRight)
	{
		m_Velocity.x = m_RunSpeed;
	}
	else
	{
		m_Velocity.x = -m_RunSpeed;
	}

}


// Soldier start attack animation
void SoldierShield::DoAttackState(float elapsedSec, Avatar* avatar)
{
	m_IsAttacking = true;

	if (m_BotSpriteChanged)
	{
		StartAttackAnimation();
	}

	if (m_IsAttackStarted && m_pBottomSprite->GetAnimationFinish())
	{
		FinishAttackAnimation(avatar);
	}

	
	if (!m_IsAttackStarted && m_pBottomSprite->GetAnimationFinish())
	{
		DoWaitState(elapsedSec, avatar);
	}

}


// Wait a moment before attacking again
void SoldierShield::DoWaitState(float elapsedSec, Avatar* avatar)
{
	m_IsAttacking = false;

	if (m_IsShieldOn)
	{
		m_Health = m_ShieldHealth;   // Soldier is protected with the shield
	}
	
	CheckDistanceAvatar(avatar->GetBotShape());

	m_SecondsWaiting += elapsedSec;

	if (m_SecondsWaiting > m_MaxTimeWait)
	{
		m_BotSpriteChanged = true;  // START attacking again
		m_SecondsWaiting = 0.f;
	}
}


void SoldierShield::StartAttackAnimation()
{
	m_Health = m_SoldierHealth;    // Soldier looses protection of his shield during this state
	m_Velocity.x = 0.f;

	// Start attack animation
	if (m_IsShieldOn)
	{
		ChangeSprite(8, 1, 8, 12.f, 54.f, 48.f, 156.f); // Soldier attack with shield
		m_IsAttackStarted = true;
	}
	else
	{
		ChangeSprite(12, 1, 12, 12.f, 58.f, 37.f, 482.f);  // Soldier without sield attacking
		m_IsAttackStarted = true;
	}

}

void SoldierShield::FinishAttackAnimation(Avatar* avatar)
{
	m_Health = m_SoldierHealth;
	m_IsAttackStarted = false;

	CheckAvatarHit(avatar);

	if (m_IsShieldOn)
	{
		ChangeSprite(9, 1, 9, 15.f, 54.f, 48.f, 210.f); // Soldier attack with shield pt2
	}
	else
	{
		ChangeSprite(11, 1, 11, 15.f, 58.f, 37.f, 540.f);  // Soldier without sield attacking pt2
	}

}

void SoldierShield::SoldierDeathAnimation()
{
	// Soldier is dying
	m_IsDying = true;
	m_Velocity.x = 0.f;
	m_pBottomSprite->UpdateValues(11, 1, 11, 8.f, 60.f, 44.f, 425.f);  // Change death animation
	m_pBottomSprite->ResetSprite();
}


// Check if the soldier hits the avatar
void SoldierShield::CheckAvatarHit(Avatar* avatar)
{
	if ( utils::IsOverlapping( m_pBottomSprite->GetDstRect(), avatar->GetTopShape() ) )
	{
		avatar->Hit();
	}
}

void  SoldierShield::UpdateFrames(float elapsedSec)
{
	bool repeat{ true };

	if (m_ActionState == ActionState::state2 || m_IsDying)
	{
		// No loop animation for this actions
		repeat = false;
	}

	m_pBottomSprite->Update(elapsedSec, repeat);
}


// Check distance between soldier and avatar and change action according to this
void SoldierShield::CheckDistanceAvatar(const Rectf& avatarShape)
{
	float avatarPos{ avatarShape.left };
	float soldierPos{};

	if (m_IsFacingRight)
	{
		soldierPos = m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width/2;
	}
	else
	{
		soldierPos = m_pBottomSprite->GetDstRect().left;
	}
	

	if ( utils::GetDistanceX(avatarPos, soldierPos) <= m_AttackDist )
	{
		// Attack state
		m_ActionState = ActionState::state2;
	}
	else
	{
		// Keep running towards the avatar
		m_ActionState = ActionState::state1;
		
	}
}

void SoldierShield::Hit()
{
	if (m_IsShieldOn)
	{
		m_ShieldHealth--;
	}

	m_Health--;

	if (m_Health == 0)
	{
		if (m_IsShieldOn)
		{
			if (m_IsAttacking)
			{
				StartFlyingShieldAnimation();
				SoldierDeathAnimation();
			}
			else
			{
				// Soldier looses his shield ( Check to which sprite to change )
				switch (m_ActionState)
				{
				case Enemy::ActionState::state1:
					ChangeSprite(12, 1, 12, 10.f, 34.f, 40.f, 368.f);  // Soldier without sield running
					break;
				case Enemy::ActionState::state2:
					ChangeSprite(12, 1, 12, 15.f, 58.f, 37.f, 482.f);  // Soldier without sield attacking
					break;
				}

				m_pBottomSprite->ResetSprite();
				StartFlyingShieldAnimation();

				m_Health = m_SoldierHealth;   // Soldier life without shield
			}
				
		}
		else
		{
			
			m_pSoundManager->PlaySoundEffect(m_pDeathSound, 0);

			SoldierDeathAnimation();
		}
	}
}


// Create a flying shield and place it in the correct pos
void SoldierShield::StartFlyingShieldAnimation()
{
	float xPos{};
	if (m_IsFacingRight)
	{
		xPos = m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width / 2;
	}
	else
	{
		xPos = m_pBottomSprite->GetDstRect().left - m_pBottomSprite->GetDstRect().width / 2;
	}

	Point2f startPos{ xPos, m_pBottomSprite->GetDstRect().bottom };
	m_pShield = new Shield(startPos);

	// Soldier doesn't have the shield anymore
	m_IsShieldOn = false;
}


void SoldierShield::CheckGameState()
{

}
