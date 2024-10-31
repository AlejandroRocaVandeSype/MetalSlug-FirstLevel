#include "pch.h"
#include "Prisoner.h"
#include "Level.h"
#include "SoundEffect.h"


Prisoner::Prisoner(const Point2f& startPos, SoundManager* sounds)
	:Enemy(GameObject::Type::prisoner, startPos, 1, Point2f{ }, 
		true, Point2f{ 0.f, -981.f}, sounds) // Type, startPos, health, speed and acceleration
	, m_IsReleased { false }
	, m_MaxTimeWait { 1.5f }
	, m_SecondsWait { }
	, m_DropItem { false }
	, m_IsItemDropped { false }
	, m_pThankYouSound{ m_pSoundManager->GetEffect("Resources/sounds/thankyou.mp3")}

{
	Initialize();
}



Prisoner::~Prisoner()
{
	delete m_pBottomSprite;
	delete m_pTopSprite;
}

void Prisoner::Initialize()
{
	m_pBottomSprite = new Sprite("Resources/sprites/prisoner.png");
	m_pBottomSprite->UpdateValues(5, 1, 5, 6.f, 48.f, 48.f, 85.f); // Iddle Prisoner
	m_pBottomSprite->SetLeftDstRect(m_StartPosition.x);
	m_pBottomSprite->SetBottomDstRect(m_StartPosition.y);
	m_pBottomSprite->UpdateLeftSrcRect();

	m_pTopSprite = new Sprite("Resources/sprites/prisoner.png");
	m_pTopSprite->UpdateValues(11, 1, 11, 8.f, 44.f, 38.f, 230.f); // Giving Object
	m_pTopSprite->SetLeftDstRect(m_StartPosition.x);
	m_pTopSprite->SetBottomDstRect(m_StartPosition.y);
	m_pTopSprite->UpdateLeftSrcRect();
}


void Prisoner::Draw() const
{

	if (!m_IsDying && !m_IsReleased)
	{
		// Prisoner captured
		m_pBottomSprite->Draw();
	}
	else
	{
		if (m_IsDying)
		{
			// Prisoner being released animation
			m_pBottomSprite->Draw();
		}
		
		if (m_IsReleased)
		{
			// Giving object animation
			m_pTopSprite->Draw();
		}
		
	}
	

}


void Prisoner::Update(float elapsedSec, Avatar* avatar, const Level* level)
{
	if (!m_IsDying && !m_IsReleased)
	{
		// Prisoner not released yet (Captured animation)
		m_pBottomSprite->Update(elapsedSec, true);
	}
	else
	{
		if (m_IsDying)
		{
			// Release the prisoner
			ReleasePrisoner(elapsedSec);
		}
		
		if (m_IsReleased)
		{
			if (!m_pBottomSprite->GetAnimationFinish())
			{
				// Finish releasing the prisoner
				ReleasePrisoner(elapsedSec);
			}
			// Prisoner released
			GiveObject(elapsedSec);
		}

	}
	
}

void Prisoner::ReleasePrisoner(float elapsedSec)
{
	m_pBottomSprite->Update(elapsedSec, false);

	if ( m_pBottomSprite->GetActFrame() == m_pBottomSprite->GetTotalFrames() - 7 )
	{
		m_IsReleased = true;
	}

	if (m_pBottomSprite->GetAnimationFinish())
	{
		m_IsDying = false;
	}

}

// Thanks the avatar by giving some object
void Prisoner::GiveObject(float elapsedSec)
{
	m_pTopSprite->Update(elapsedSec, false);

	if ( m_pTopSprite->GetAnimationFinish() )
	{
		
		if ( !m_IsItemDropped )
		{
			m_DropItem = true;		// Item needs to be drop for the player
			m_IsItemDropped = true;

		}
			

		m_SecondsWait += elapsedSec;

		if ( m_SecondsWait >= m_MaxTimeWait )
		{
			
			m_IsReleased = false;
			m_IsActive = false;
			m_IsDead = true;
			m_pTopSprite->ResetAnimationFinish(false);
			m_pTopSprite->ResetActFrame();
		}
		
	}
}

void Prisoner::Hit()
{
	m_Health--;

	if (m_Health == 0)
	{
		m_IsDying = true;
		m_pBottomSprite->UpdateValues(14, 1, 14, 8.f, 48.f, 48.f, 136.f);  // Change to release animation
		m_pBottomSprite->ResetActFrame();

		m_pSoundManager->PlaySoundEffect(m_pThankYouSound, 0);
		
	}
	

}

// Check if avatar shape overlaps with the prisoner shape in order to release him
void Prisoner::CheckAvatarRelease(const Rectf& avatarShape)
{

	if (utils::IsOverlapping(m_pBottomSprite->GetDstRect(), avatarShape))
	{
		// Avatar collides with the prisoner --> Release him
		Hit();

	}

}

void Prisoner::ItemDropped()
{
	m_DropItem = false;
}

bool Prisoner::GetDropItem() const
{
	return m_DropItem;
}

void Prisoner::CheckGameState()
{

}