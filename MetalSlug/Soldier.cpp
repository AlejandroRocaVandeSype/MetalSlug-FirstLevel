#include "pch.h"
#include "Soldier.h"
#include "Level.h"
#include "Avatar.h"
#include "Grenade.h"
#include "SoundEffect.h"
#include <iostream>



Soldier::Soldier(const Point2f& startPos, bool comingFromRight, SoundManager* soundManager)
	:Enemy(GameObject::Type::soldier, startPos, 1, Point2f{ 150.f, 0.f }, 
		comingFromRight, Point2f{0.f, g_Gravity}, soundManager)   // Type, starting position, health, speed and accelerat
	, m_RunSpeed { 200.f }
	, m_RunAwayDist { 200.f }
	, m_ThrowGrenadeDist { 300.f }
	, m_MaxTimeRunAway { 3.f }
	, m_MaxTimeWait { 1.f }
	, m_Seconds { -1.f }
	, m_IsReset { false }
	, m_pGrenade { }
	, m_pDeathSound{ soundManager->GetEffect(GetDeathSound())} // Choose randomly a death effect for the soldier

{
	Initialize();
}


Soldier::~Soldier()
{
	delete m_pBottomSprite;

	if (m_pGrenade != nullptr)
	{
		// Only delete if it exits
		delete m_pGrenade;
	}

}


void Soldier::Draw() const
{

	switch (m_ActionState)
	{
		case Enemy::ActionState::state2:
			if (!m_IsFacingRight)
			{
				// Flip to the left
				glPushMatrix();

				m_pBottomSprite->FlipSprite();
				m_pBottomSprite->Draw();

				glPopMatrix();
			}
			else
			{
				m_pBottomSprite->Draw();
			}
			break;
		case Enemy::ActionState::state3:
		case Enemy::ActionState::state1:
		case Enemy::ActionState::state4:
		case Enemy::ActionState::state5:
			if (m_IsFacingRight)
			{
				// Flip to the left
				glPushMatrix();

				m_pBottomSprite->FlipSprite();
				m_pBottomSprite->Draw();

				glPopMatrix();
			}
			else
			{
				m_pBottomSprite->Draw();
			}
			break;

		default:
			m_pBottomSprite->Draw();
			break;
	}

	if (m_pGrenade != nullptr)
	{
		if (m_pGrenade->GetIsActive())
		{
			m_pGrenade->Draw();
		}
	}

}

void Soldier::Update(float elapsedSec, Avatar* avatar, const Level* level)
{
	CheckPreviousAction();

	if (!m_IsDying)
	{
		// Soldier still alive --> Do AI
		switch (m_ActionState)
		{
		case Enemy::ActionState::state1:
			// Walking
			DoWalkingState(elapsedSec, avatar->GetBotShape());
			break;
		case Enemy::ActionState::state2:
			// Attack
			DoAttackState(elapsedSec, avatar->GetBotShape());
			break;
		case Enemy::ActionState::state3:
			// Running Away
			DoRunAwayState(elapsedSec, avatar->GetBotShape());
			break;
		case Enemy::ActionState::state5:
			// Waiting
			DoWaitingState(elapsedSec, avatar->GetBotShape());
			break;
		}
	}
	else
	{
		// Soldier Hit --> Kill him
		KillSoldier(elapsedSec);
	}
	

	UpdateFrames(elapsedSec);
	Falling(elapsedSec, level);
	Move(elapsedSec);
	level->HandleCollision(m_pBottomSprite->GetDstRect(), m_Velocity);

	if (m_pGrenade != nullptr)
	{
		if (m_pGrenade->GetIsActive())
		{
			// Grenade throwed
			m_pGrenade->Update(elapsedSec, avatar, this, level->GetVertices());
		}
		
			
		
	}
		
}



void Soldier::UpdateFrames(float elapsedSec)
{
	bool repeat{ true };

	if (m_ActionState == ActionState::state2 || m_IsDying)
	{
		// No loop animation for this actions
		repeat = false;
	}

	m_pBottomSprite->Update(elapsedSec, repeat);
}




// Walking State --> Soldier walks into direction of the avatar
void Soldier::DoWalkingState(float elapsedSec, const Rectf& avatarShape)
{
	if (m_BotSpriteChanged)
	{
		ChangeSprite(12, 1, 12, 10.f, 34.f, 40.f, 40.f);
	}
	
	CheckDistanceAvatar(avatarShape);

	CheckFacingRight(avatarShape);

	// Move in the direction of the avatar
	if (m_IsFacingRight)
	{
		m_Velocity.x = m_Speed.x;
	}
	else
	{
		m_Velocity.x = -m_Speed.x;
	}
	
}

// Throwing Grenade State --> Throws a grenade towards direction avatar 
// then wait a couple of seconds to repeat the process
void Soldier::DoAttackState(float elapsedSec, const Rectf& avatarShape)
{
	if (m_BotSpriteChanged)
	{
		ChangeSprite(14, 1, 14, 15.f, 40.f, 46.f, 191.f);
		m_Velocity.x = 0.f;
	}
	
	if (m_pGrenade == nullptr)
	{
		m_pGrenade = new Grenade(m_pSoundManager);
	}

	if (!m_pGrenade->GetIsActive())
	{
		// Grenade available
		ActivateGrenade();
	}
	

	if (m_pBottomSprite->GetAnimationFinish())
	{
		// Only do this when animation of throwing grenade has finished

		CheckDistanceAvatar(avatarShape);

		float avatarPos{ avatarShape.left };
		float soldierPos{ m_pBottomSprite->GetDstRect().left };

		if (utils::GetDistanceX(avatarPos, soldierPos) <= m_ThrowGrenadeDist)
		{
			// Waiting state until throw next grenade
			m_ActionState = ActionState::state5;
		}
		else
		{
			// Follow avatar
			m_ActionState = ActionState::state1;
		}
		m_pBottomSprite->ResetActFrame();

		if (m_SecondsWaiting >= m_MaxTimeWait)
		{
			if (!m_IsReset)
			{
				m_pBottomSprite->ResetSprite();
				m_IsReset = true;
			}
		}
	}

	CheckFacingRight(avatarShape);

}

// Run away from avatar state --> Run away from the avatar for a few seconds,
// then check position of the avatar
void Soldier::DoRunAwayState(float elapsedSec, const Rectf& avatarShape)
{
	if (m_BotSpriteChanged)
	{
		ChangeSprite(12, 1, 12, 10.f, 40.f, 38.f, 288.f);
	}

	// RunAway until time has finished
	m_Seconds += elapsedSec;
	if ( m_Seconds >= m_MaxTimeRunAway )
	{
		m_Seconds = 0.f;
		CheckDistanceAvatar(avatarShape);
	}
	
	CheckFacingRight(avatarShape);

	if (m_IsFacingRight)
	{
		m_Velocity.x = - m_RunSpeed;
	}
	else
	{
		m_Velocity.x = m_RunSpeed;
	}

}

// Waiting State --> Wait until throw next grenade
void Soldier::DoWaitingState(float elapsedSec, const Rectf& avatarShape)
{
	if (m_BotSpriteChanged)
	{
		ChangeSprite(6, 1, 6, 10.f, 34.f, 40.f, 140.f);
	}

	float avatarPos{ avatarShape.left };
	float soldierPos{ m_pBottomSprite->GetDstRect().left };

	if (utils::GetDistanceX(avatarPos, soldierPos) <= m_RunAwayDist)
	{
		// Run Away
		m_ActionState = ActionState::state3;
	}

	// Waiting time
	m_SecondsWaiting += elapsedSec;
	if (m_SecondsWaiting >= m_MaxTimeWait)
	{
		// Attack
		m_ActionState = ActionState::state2;
	}
	
}

// Activate the grenate and put it on the correct position to throw it
void Soldier::ActivateGrenade()
{
	
	if (m_pBottomSprite->GetActFrame() == m_pBottomSprite->GetTotalFrames() - 4)
	{
		// Activate grenade
		m_pGrenade->SetIsActive(true);

		Point2f grenadePos{ };

		grenadePos.y = m_pBottomSprite->GetDstRect().bottom + m_pBottomSprite->GetDstRect().height / 1.5f;
		grenadePos.x = m_pBottomSprite->GetDstRect().left;

		m_pGrenade->SetStartPos(grenadePos);

		m_SecondsWaiting = 0.f;
		m_IsReset = false;
	}
}


void Soldier::Move(float elapsedSec)
{
	m_pBottomSprite->SetLeftDstRect(m_pBottomSprite->GetDstRect().left + m_Velocity.x * elapsedSec);
	m_pBottomSprite->SetBottomDstRect(m_pBottomSprite->GetDstRect().bottom + m_Velocity.y * elapsedSec);
}



// Check distance between soldier and avatar and change action according to this
void Soldier::CheckDistanceAvatar(const Rectf& avatarShape)
{
	float avatarPos{ avatarShape.left };
	float soldierPos{ m_pBottomSprite->GetDstRect().left };

	if (utils::GetDistanceX(avatarPos, soldierPos) <= m_RunAwayDist)
	{
		// Run Away State
		m_ActionState = ActionState::state3;
	}
	else
	{
		if (utils::GetDistanceX(avatarPos, soldierPos) <= m_ThrowGrenadeDist)
		{
			// Attack State
			m_ActionState = ActionState::state2;
			m_Velocity.x = 0.f;
		}
		else
		{
			// Walking torwards avatar state
			m_ActionState = ActionState::state1;
		}

	}
}

void Soldier::Hit()
{
	m_Health--;

	if (m_Health == 0)
	{
		// Soldier is dead 
		m_IsDying = true;
		m_Velocity.x = 0.f;
		m_pBottomSprite->UpdateValues(11, 1, 11, 8.f, 60.f, 44.f, 357.f);  // Change death animation
		m_pBottomSprite->ResetSprite();
		m_ActionState = ActionState::state4;  // Death state
		
		m_pSoundManager->PlaySoundEffect(m_pDeathSound, 0);
		
	}

}





void Soldier::CheckGameState()
{

}


void Soldier::Initialize()
{
	m_pBottomSprite = new Sprite("Resources/sprites/enemies/rebelSoldier.png");
	m_pBottomSprite->UpdateValues(12, 1, 12, 10.f, 34.f, 40.f, 40.f); // Walking Soldier
	m_pBottomSprite->SetLeftDstRect(m_StartPosition.x);
	m_pBottomSprite->SetBottomDstRect(m_StartPosition.y);
	m_pBottomSprite->UpdateLeftSrcRect();

}


bool Soldier::GetFacingRight() const
{
	return m_IsFacingRight;
}