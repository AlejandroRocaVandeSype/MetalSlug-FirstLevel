#include "pch.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Avatar.h"
#include "utils.h"
#include "SoundEffect.h"
#include <iostream>



Boss::Boss(const Point2f& startPos, bool comingFromRight, SoundManager* soundManager)
	:Enemy(GameObject::Type::boss, startPos, 100, Point2f{ },
		comingFromRight, Point2f{ 0.f, g_Gravity }, soundManager)
	, m_FightState { State::sleeping }
	, m_PreviousState { State::sleeping }
	, m_Seconds { }
	, m_NrOfShots { }
	, m_NrOfChargings { }
	, m_pWaterBullet{ new BossBullet(false, Point2f{ startPos.x - 40.f, startPos.y + 40.f }, soundManager) }
	, m_pAirBullets{ }
	, m_BulletAirSpeed{ 200.f, 500.f }
	, m_CloathSpeed { 150.f }
	, m_NextFrame{ 10 }
	, m_pExplosion{ }
	, m_pWaterShoot{ }
	, m_pAirShoot{ }
	, m_pMoving{ }
{

	Initialize();
}


void Boss::Initialize()
{
	m_Velocity.y = 1500.f;
	// BOSS
	m_pBottomSprite = new Sprite("Resources/sprites/enemies/Boss/boss.png");
	m_pBottomSprite->UpdateValues(1, 1, 1, 20.f, 90.f, 70.f, 210.f);  // Iddle
	m_pBottomSprite->SetLeftDstRect(m_StartPosition.x);
	m_pBottomSprite->SetBottomDstRect(m_StartPosition.y);
	m_pBottomSprite->UpdateLeftSrcRect();

	// CLOTH
	m_pTopSprite = new Sprite("Resources/sprites/enemies/Boss/Cloth.png");
	m_pTopSprite->UpdateValues(1, 1, 1, 10.f, 120.f, 93.f, 108.f);  // Cloth iddle
	m_pTopSprite->SetLeftDstRect(m_StartPosition.x + 120.f);
	m_pTopSprite->SetBottomDstRect(m_StartPosition.y - 30.f);
	m_pTopSprite->UpdateLeftSrcRect();


	// Bullets
	for (int i{}; i < 3; i++)
	{
		m_pBullets.push_back(new BossBullet(true, Point2f{ m_StartPosition.x + 130.f, m_StartPosition.y + 130.f }, m_pSoundManager));
	}

	m_pBullets.push_back(m_pWaterBullet);

	// Sounds
	m_pExplosion = m_pSoundManager->GetEffect("Resources/sounds/explosion2.wav");
	m_pWaterShoot = m_pSoundManager->GetEffect("Resources/sounds/WaterShoot.wav");
	m_pAirShoot = m_pSoundManager->GetEffect("Resources/sounds/AirShoot.wav");
	m_pMoving = m_pSoundManager->GetEffect("Resources/sounds/BossMoving.wav");
}

Boss::~Boss()
{
	delete m_pBottomSprite;
	delete m_pTopSprite;

	for (BossBullet* spr : m_pBullets)
	{
		delete spr;
	}

}


 void Boss::Draw() const
{
	 m_pBottomSprite->Draw();

	 if (m_FightState == State::sleeping || m_FightState == State::starting || m_FightState == State::dying)
	 {
		 m_pTopSprite->Draw();
	 }

	 
	 for (BossBullet* spr : m_pBullets)
	 {
		 if (spr->GetIsActive())
		 {
			 spr->Draw();
		 }
	 }
	
}


void Boss::Hit()
{
	m_Health--;

	if (m_Health == 0)
	{
		// Boss is dead
		m_IsDying = true;
		m_FightState = State::dying;
		m_pBottomSprite->UpdateValues(1, 1, 1, 20.f, 90.f, 70.f, 420.f);
		m_pBottomSprite->ResetSprite();

		delete m_pTopSprite;    // Free memory 
		// Reserve memory for the explosion animation
		m_pTopSprite = new Sprite("Resources/sprites/enemies/Explosion.png");
		m_pTopSprite->UpdateValues(5, 1, 5, 10.f, 35.2f, 64.f, 64.f, 2.7f);
		m_pTopSprite->SetLeftDstRect(m_StartPosition.x + 120.f);
		m_pTopSprite->SetBottomDstRect(m_StartPosition.y);
		m_pTopSprite->UpdateLeftSrcRect();
	}
	else
	{
		if (m_FightState == State::sleeping)
		{
			m_pTopSprite->UpdateValues(15, 1, 15, 7.f, 120.f, 103.f, 230.f);  // Cloth flying away
			m_pTopSprite->UpdateLeftSrcRect();

			// Start fight if it hasn't started yet
			m_FightState = State::starting;
			m_PreviousState = State::sleeping;
		}
	}

	
}

void Boss::CheckGameState()
{

}

void Boss::Update(float elapsedSec, Avatar* avatar, const Level* level)
{

	switch (m_FightState)
	{
	case Boss::State::starting:   // Boss starting fight 
		StartFight(elapsedSec);
		break;
	case Boss::State::firingUp:
		FiringUp(elapsedSec);
		break;
	case Boss::State::waiting:
		Waiting(elapsedSec);
		break;
	case Boss::State::chargingShot:
		ChargingShot(elapsedSec);
		break;
	case Boss::State::firingBot:
		FiringBot(elapsedSec);
		break;
	case Boss::State::dying:
		Dying(elapsedSec);
		break;
	}

	for (BossBullet* spr : m_pBullets)
	{
		if (spr->GetIsActive())
		{
			spr->Update(elapsedSec);
		}
	}
	

}

void Boss::StartFight(float elapsedSec)
{
	m_pTopSprite->Update(elapsedSec, false);

	if (m_pTopSprite->GetAnimationFinish())
	{
		m_pTopSprite->SetLeftDstRect(m_pTopSprite->GetDstRect().left + m_CloathSpeed * elapsedSec);

		m_Seconds += elapsedSec;

		if (m_Seconds >= 2.f)
		{
			m_FightState = State::firingUp;
			m_PreviousState = State::starting;
			m_Seconds = 0.f;
		}
	}
}

void Boss::FiringUp(float elapsedSec)
{

	if (m_PreviousState == State::waiting || m_PreviousState == State::starting)
	{
		m_pBottomSprite->ResetSprite();
		m_pBottomSprite->UpdateValues(24, 1, 24, 11.f, 90.f, 70.f, 70.f);   // Firing up

		m_pSoundManager->PlaySoundEffect(m_pMoving, 0);

		m_PreviousState = State::firingUp;
	}


	m_pBottomSprite->Update(elapsedSec, false);

	if ( m_pBottomSprite->GetAnimationFinish() )
	{
		m_NrOfShots++;		 // One shot done

		m_FightState = State::waiting;
		m_PreviousState = State::firingUp;
		
	}
	else
	{
		if (m_pBottomSprite->GetActFrame() == m_NextFrame )
		{
			SelectNextShotFrame();

			m_pSoundManager->PlaySoundEffect(m_pAirShoot, 0);

			for (BossBullet* spr : m_pBullets)
			{
				if (spr->GetBulletType() == BossBullet::BulletType::airBullet)
				{
					if (!spr->GetIsActive())
					{
						spr->SetIsActive(true);
						break;
					}
				}
			}
		}
	}
}

void Boss::SelectNextShotFrame()
{
	if (m_NextFrame == 10)
	{
		m_NextFrame = 15;
	}
	else
	{
		if (m_NextFrame == 15)
		{
			m_NextFrame = 20;
		}
		else
		{
			if (m_NextFrame == 20)
			{
				m_NextFrame = 10;
			}
		}
	}
}

void Boss::Waiting(float elapsedSec)
{
	if (m_PreviousState == State::firingUp)
	{
		m_pBottomSprite->ResetSprite();
		m_pBottomSprite->UpdateValues(9, 1, 9, 11.f, 90.f, 70.f, 140.f);   // Going back firing up
		m_PreviousState = State::waiting;
	}
	else
	{
		if (m_PreviousState == State::firingBot)
		{
			m_pBottomSprite->ResetSprite();
			m_pBottomSprite->UpdateValues(1, 1, 1, 20.f, 90.f, 70.f, 210.f);  // Iddle
			m_PreviousState = State::waiting;
		}
	}
	
	m_pBottomSprite->Update(elapsedSec, false);


	if (m_pBottomSprite->GetAnimationFinish())
	{
		if (m_NrOfShots >= 3)
		{
			// Change to firing Bottom
			m_FightState = State::chargingShot;
			m_PreviousState = State::waiting;
			m_NrOfShots = 0;
		}
		else
		{
			// Change to firing Up
			m_FightState = State::firingUp;
			m_PreviousState = State::waiting;
		}

	
	}

	
}

void Boss::ChargingShot(float elapsedSec)
{
	if (m_PreviousState == State::waiting)
	{
		m_pBottomSprite->ResetSprite();
		m_pBottomSprite->UpdateValues(3, 1, 3, 20.f, 90.f, 70.f, 350.f);	// Charging shot

		m_PreviousState = State::chargingShot;
	}

	m_pBottomSprite->Update(elapsedSec, false);

	if (m_pBottomSprite->GetAnimationFinish())
	{
		m_NrOfChargings++;

		if (m_NrOfChargings >= 10)
		{
			// Fire Bullet
			m_NrOfChargings = 0;
			m_FightState = State::firingBot;
			m_PreviousState = State::chargingShot;
		}

		m_pBottomSprite->ResetSprite();
	}

}

void Boss::FiringBot(float elapsedSec)
{
	if (m_PreviousState == State::chargingShot)
	{
		m_pBottomSprite->ResetSprite();
		m_pBottomSprite->UpdateValues(11, 1, 11, 10.f, 90.f, 70.f, 280.f);  // Firing Bot
		m_pSoundManager->PlaySoundEffect(m_pWaterShoot, 0);

		m_PreviousState = State::firingBot;
		

		for (BossBullet* spr : m_pBullets)
		{
			if (spr->GetBulletType() == BossBullet::BulletType::groundBullet)
			{
				spr->SetIsActive(true);
			}
		}

	}

	m_pBottomSprite->Update(elapsedSec, false);

	if (m_pBottomSprite->GetAnimationFinish())
	{
		m_FightState = State::waiting;
		m_PreviousState = State::firingBot;
	}

}

void Boss::Dying(float elapsedSec)
{
	m_Seconds += elapsedSec;

	m_pTopSprite->Update(elapsedSec, false);

	if (m_Seconds > 4.f)
	{
		m_GameState = GameStage::end;
	}
	else
	{
		DyingAnimation(elapsedSec);
	}


	

}

void  Boss::DyingAnimation(float elapsedSec)
{
	if (m_pTopSprite->GetAnimationFinish())
	{
		// Random explosions around the boss
		m_pSoundManager->PlaySoundEffect(m_pExplosion, 0);
		m_pTopSprite->ResetSprite();
		float xPos{ float((rand() % 100) + 100) };
		float yPos{ float((rand() % 80)) };
		m_pTopSprite->SetLeftDstRect(m_StartPosition.x + xPos);
		m_pTopSprite->SetBottomDstRect(m_StartPosition.y + yPos);
		m_pTopSprite->UpdateLeftSrcRect();
	}
}

// Check if bullet hit avatar
void Boss::CheckHitBullet(Avatar* avatar)
{
	for (BossBullet* spr : m_pBullets)
	{
		if (spr->GetIsActive())
		{
			if (utils::IsOverlapping(avatar->GetHitBox(), spr->GetTopShape()))
			{
				spr->Hit();
				avatar->Hit();
			}
		}
		
	}
}

Point2f Boss::GetStartPos() const
{
	return m_StartPosition;
}
