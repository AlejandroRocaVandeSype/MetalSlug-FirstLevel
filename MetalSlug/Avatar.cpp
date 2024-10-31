#include "pch.h"
#include "Avatar.h"
#include "Texture.h"
#include "Level.h"
#include "BulletManager.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include <iostream>


Avatar::Avatar(SoundManager* sounds)
	: GameObject(GameObject::Type::avatar, Point2f{ 100 * g_Scale, 200.f }, Point2f{ m_NormalSpeed, m_JumpSpeed }
		, false , sounds, Point2f{ 0.f, g_Gravity + g_Gravity/2 })  // Type, startPos, speed, soundManager and acceleration
	, m_SlowSpeed{ 90.f }
	, m_JumpSpeed{ 650.f }
	, m_NormalSpeed { 260.f }
	, m_IsMovingRight{ true }
	, m_TopActionState{ TopActionState::jumping }
	, m_BotActionState{ BotActionState::jumping }
	, m_ActTopAnimation{ Animations::jumping }
	, m_ActBotAnimation{ Animations::jumping }
	, m_PrevTopAnimation{ m_ActTopAnimation }
	, m_PrevBotAnimation{ m_ActBotAnimation }
	, m_IsOnGround{ }
	, m_Offset{ }
	, m_IsShooting{ false }
	, m_IsMoving{ false }
	, m_IsImmortal { false }
	, m_IsBlink { false }
	, m_pBulletManager{ new BulletManager }
	, m_NrOfBullets { 20 }
	, m_MaxTimeRespawn { 1.5f }
	, m_MaxTimeImmortal { 2.f }
	, m_SecondsImmortal { }
	, m_SecondsRespawn {  }
	, m_CameraPos{ }
	, m_CountNrFrames { }
	, m_pDeathSound { m_pSoundManager->GetEffect("Resources/sounds/HeroDies.wav") }
	, m_pPistolFire{ m_pSoundManager->GetEffect("Resources/Sounds/PistolFire.wav") }
	, m_NrLifes { 2 }
	, m_AreLifesLeft { true }
{

	Initialize();

}

Avatar::~Avatar()
{

	delete m_pBottomSprite;
	delete m_pTopSprite;
	delete m_pBulletManager;
}


void Avatar::Initialize()
{
	
	// Marco Body (Iddle)
	m_pTopSprite = new Sprite( "Resources/sprites/marco/MarcoBody.png" );
	m_pTopSprite->UpdateValues(4, 1, 4, 7.f, 33.f, 29.f, 29.f);
	m_pTopSprite->SetLeftDstRect(m_StartPosition.x);
	m_pTopSprite->SetBottomDstRect(m_StartPosition.y);
	
	m_pTopSprite->UpdateLeftSrcRect();

	// Marco Legs (Iddle)
	m_pBottomSprite = new Sprite("Resources/sprites/marco/MarcoLegs.png");
	m_pBottomSprite->UpdateValues(1, 1, 1, 15.f, 25.f, 15.f, 15.f);
	m_pBottomSprite->SetLeftDstRect(m_pTopSprite->GetDstRect().left);
	m_pBottomSprite->SetBottomDstRect(m_pTopSprite->GetDstRect().bottom - m_pBottomSprite->GetFrameHeight() );

	m_pBottomSprite->UpdateLeftSrcRect();

	InitBulletManager();


}

void Avatar::InitBulletManager()
{

	Point2f startPos{ m_pBottomSprite->GetDstRect().left , m_pBottomSprite->GetDstRect().bottom };

	for (int i{ }; i < m_NrOfBullets; i++)
	{
		m_pBulletManager->AddBullet(startPos);
	}

}


void Avatar::Draw() const
{

	if (!m_IsImmortal)  // Player not immortal
	{
		CheckFlipSprite();
	}
	else
	{
		if (m_IsImmortal && m_IsBlink)  // Player Immortal
		{
			CheckFlipSprite();
		}
		
	}
	
	m_pBulletManager->Draw();

}

void Avatar::CheckFlipSprite() const
{
	if (!m_IsMovingRight)
	{
		// Moving left --> Flip sprite to the left
		glPushMatrix();

		m_pBottomSprite->FlipSprite();

		DrawAvatar();

		glPopMatrix();
	}
	else
	{
		DrawAvatar();
	}
}


void Avatar::DrawAvatar() const
{
	
	if ( m_TopActionState != TopActionState::crawling  && m_TopActionState != TopActionState::death)
	{
		// Not Crawling --> Draw both parts of the sprite
		m_pBottomSprite->Draw();
		m_pTopSprite->Draw();

	}
	else
	{
		// Crawling / Death --> Draw only the bottom sprite
		m_pBottomSprite->Draw();
	}
	
}

void Avatar::Update( float elapsedSeconds, const Level* level, const Rectf& cameraPos)
{

	m_CameraPos = cameraPos;

	if (!m_IsDead)
	{
		HandleInput();
	}
	
	if (m_IsImmortal)
	{
		AvoidDamage(elapsedSeconds);

	}

	UpdateFrames(elapsedSeconds);
	
	UpdateTopSrcRect();
	UpdateBotSrcRect();


	
	if (!level->IsOnGround(m_pBottomSprite->GetDstRect(), m_Velocity))
	{
		AvatarFalling(elapsedSeconds);
		m_IsOnGround = false;
	}
	else
	{
		m_IsOnGround = true;
	}

	Move(elapsedSeconds);
	level->HandleCollision(m_pBottomSprite->GetDstRect(), m_Velocity);

	if (m_Velocity.x == 0)
	{
		m_IsMoving = false;
	}
	
	if (!m_IsDead)
	{
		CheckGameState();
	}

	m_pBulletManager->Update(elapsedSeconds, this);

}

// Update the Source Rectangle of the Top Sprite
void Avatar::UpdateTopSrcRect()
{
	m_Offset = 0;

	switch ( m_TopActionState )
	{
	case Avatar::TopActionState::iddle:
		
		if ( m_IsMoving )
		{
			// Marco Body (Moving)
			m_pTopSprite->UpdateValues( 12, 1, 12, 25.f, 40.f, 29.f, 87.f );
			m_Offset = 10;
		}
		else
		{
			
			// Marco Body (Iddle)
			m_pTopSprite->UpdateValues( 3, 1, 3, 6.f, 33.f, 29.f, 29.f );
			m_pTopSprite->SetLeftDstRect( m_pBottomSprite->GetDstRect().left );
			m_Offset = 7;
			

			
		}

		break;

	case Avatar::TopActionState::crawling:
		// Crawling + Not Moving
		m_pTopSprite->UpdateValues(4, 1, 4, 5.f, 36.f, 24.f, 144.f);
		break;

	case Avatar::TopActionState::jumping:

		// Marco Body (JUMPING)
		m_pTopSprite->UpdateValues(5, 1, 5, 15.f, 32.f, 25.f, 54.f);
		m_pTopSprite->SetLeftDstRect(m_pBottomSprite->GetDstRect().left - 18);
		m_Offset = 35;
		break;

	case Avatar::TopActionState::shooting:

		// Marco Body ( Shooting)
		m_pTopSprite->UpdateValues( 10, 1, 10, 15.f, 60.f, 27.f, 275.f );
		m_pTopSprite->SetLeftDstRect( m_pBottomSprite->GetDstRect().left - 15 );

		if ( m_BotActionState == BotActionState::jumping )
		{
			m_Offset = 28;
		}
		else
		{
			m_Offset = 14;
		}
		break;

	case Avatar::TopActionState::pointingUp:

		m_pTopSprite->UpdateValues( 6, 1, 6, 15.f, 40.f, 29.f, 118.f );
		m_pTopSprite->SetLeftDstRect( m_pBottomSprite->GetDstRect().left - 15 );
		m_Offset = 16;
		break;

	case Avatar::TopActionState::shootingUp:

		m_pTopSprite->UpdateValues( 10, 1, 10, 15.f, 40.f, 70.f, 248.f );
		m_Offset = 14;
		break;
	}


	// Previous Action State different?
	if ( m_TopSpriteChanged )
	{
		ResetSprite( m_pTopSprite, true );
	}
}


// Update the Source Rectangle of the Sprite from Marco Legs (Bottom Sprite)
void Avatar::UpdateBotSrcRect()
{

	switch ( m_BotActionState )
	{
	case Avatar::BotActionState::iddle:

		if ( m_IsMoving )
		{
			// Marco Legs (Moving)
			m_pBottomSprite->UpdateValues( 12, 1, 12, 23.f, 30.f, 19.f, 44.f );
			m_pTopSprite->SetLeftDstRect( m_pBottomSprite->GetDstRect().left ); // (correct the position of the sprite)
		}
		else
		{
			// Marco Legs (Iddle)
			m_pBottomSprite->UpdateValues( 1, 1, 1, 15.f, 25.f, 15.f, 15.f );
		}
		break;

	case Avatar::BotActionState::crawling:

		if ( m_IsMoving )
		{
			// Crawling + Moving
			m_pBottomSprite->UpdateValues( 7, 1, 7, 15.f, 36.f, 24.f, 168.f );
		}
		else
		{
			// Crawling + Not Moving
			m_pBottomSprite->UpdateValues( 4, 1, 4, 6.f, 36.f, 24.f, 144.f );
		}
		break;

	case Avatar::BotActionState::jumping:

		m_pBottomSprite->UpdateValues( 6, 1, 6, 15.f, 25.f, 25.f, 100.f );
		break;

	case Avatar::BotActionState::shooting:  // Crawling Shooting

		m_pBottomSprite->UpdateValues( 7, 1, 7, 15.f, 50.f, 28.f, 197.f );
		break;

	case Avatar::BotActionState::death:
		m_pBottomSprite->UpdateValues(16, 1, 16, 15.f, 40.f, 40.f, 242.f);
		break;
	}

	// Previous Action State different?
	if ( m_BotSpriteChanged )
	{
		ResetSprite( m_pBottomSprite, false );
	}
}

// Update frames of the sprite
void Avatar::UpdateFrames( float elapsedSeconds )
{
	// To know if the current animation needs to be repeat or not
	bool repeatBot{ true };  
	bool repeatTop{ true };

	switch (m_TopActionState)
	{
		case Avatar::TopActionState::jumping:
			repeatTop = false;
			break;
		case Avatar::TopActionState::shooting:
			repeatTop = false;
			break;
		case Avatar::TopActionState::pointingUp:
			repeatTop = false;
			break;
		case Avatar::TopActionState::shootingUp:
			repeatTop = false;
			break;
	}
	
	switch (m_BotActionState)
	{
		case Avatar::BotActionState::jumping:
			repeatBot = false;
			break;

		case Avatar::BotActionState::shooting:
			repeatBot = false;
			break;

		case Avatar::BotActionState::death:
			repeatBot = false;
			break;
	}

	// Update active frames
	m_pTopSprite->Update( elapsedSeconds, repeatTop );
	m_pBottomSprite->Update( elapsedSeconds, repeatBot );
	

	// Check if the shooting animation has finished


		if( m_IsShooting && m_pTopSprite->GetAnimationFinish() )
		{
			if (m_IsOnGround)
			{
				// Only reset shoot animation when we are On the ground
				ResetSprite(m_pTopSprite, true);
				m_IsShooting = false;
				m_TopActionState = TopActionState::iddle;
			}
		
		
		}

		/*
	if (m_IsShooting && m_pBottomSprite->GetAnimationFinish())
	{
		if (m_IsOnGround)
		{
			// Only reset shoot animation when we are On the ground
			ResetSprite(m_pBottomSprite, false);
			m_IsShooting = false;
		
		}
	}
	*/

	
	// Check if death animation has finished in order to respawn
	if (m_IsDead && m_pBottomSprite->GetAnimationFinish())
	{

		Respawn(elapsedSeconds);

	}
	
	
	
}


// Reset the Active frame to zero when the sprite animation has changed
void Avatar::ResetSprite( Sprite* sprite, bool topSprite )
{
	sprite->ResetActFrame();
	sprite->UpdateLeftSrcRect();
	sprite->ResetAnimationFinish(false);

	// Check wich sprite changed
	if ( topSprite )
	{
		m_TopSpriteChanged = false;

	}
	else
	{
		m_BotSpriteChanged = false;
	}
}

void Avatar::HandleInput()
{
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );

	if ( !m_IsMoving  && m_IsOnGround )
	{
		// NO MOVEMENT
		m_TopActionState = TopActionState::iddle;
		m_BotActionState = BotActionState::iddle;
		m_ActTopAnimation = Animations::iddle;
		m_ActBotAnimation = Animations::iddle;
		m_IsMoving = false;

	}


	
	if ( pStates[SDL_SCANCODE_C] )
	{
		// JUMP
		if (m_IsOnGround)
		{
			// Only jump when on ground
			m_TopActionState = TopActionState::jumping;
			m_BotActionState = BotActionState::jumping;
			m_ActTopAnimation = Animations::jumping;
			m_ActBotAnimation = Animations::jumping;
			m_Velocity.y = m_JumpSpeed;
			
		}

	}

	if ( m_BotActionState != BotActionState::jumping )
	{
		// Only if not jumping 

		if ( pStates[SDL_SCANCODE_UP] )
		{
			// POINTING WEAPON UPWARDS
			m_TopActionState = TopActionState::pointingUp;
			m_ActTopAnimation = Animations::pointingUp;

		}
		else
		{
			if ( pStates[SDL_SCANCODE_DOWN] )
			{
				// CRAWLING
				m_TopActionState = TopActionState::crawling;
				m_BotActionState = BotActionState::crawling;
				m_ActTopAnimation = Animations::crawlingIddle;
				m_ActBotAnimation = Animations::crawlingIddle;
			}

			if (pStates[SDL_SCANCODE_X]  && pStates[SDL_SCANCODE_DOWN] )
			{
				// CRAWLING
				m_TopActionState = TopActionState::crawling;
				m_BotActionState = BotActionState::crawling;
				m_ActTopAnimation = Animations::crawlingIddle;
				m_ActBotAnimation = Animations::crawlingIddle;
			}
			
		}
	}


	if ( pStates[SDL_SCANCODE_RIGHT] )
	{
		// MOVING RIGHT
		m_IsMoving = true;
		m_IsMovingRight = true;
		CheckCrawling();

		if ( m_TopActionState != TopActionState::crawling)
		{
		
			m_ActTopAnimation = Animations::movingRight;
			m_ActBotAnimation = Animations::movingRight;
		}
		else
		{
			m_ActTopAnimation = Animations::crawlingRight;
			m_ActBotAnimation = Animations::crawlingRight;
			
		}
		


		if ( ( m_BotActionState == BotActionState::jumping ) && m_IsOnGround )
		{
			
			// Only change the sprite when avatar is on ground after jumping
			m_TopActionState = TopActionState::iddle;
			m_BotActionState = BotActionState::iddle;
			m_ActTopAnimation = Animations::iddle;
			m_ActBotAnimation = Animations::iddle;

		}

		if ( pStates[SDL_SCANCODE_C] )
		{
			// JUMP + MOVE RIGHT
			if ( m_IsOnGround )
			{
				
				m_TopActionState = TopActionState::jumping;
				m_BotActionState = BotActionState::jumping;
				m_ActTopAnimation = Animations::jumping;
				m_ActBotAnimation = Animations::jumping;
				m_Velocity.y = m_JumpSpeed;

			}

		}
		
	}
	else
	{
		if ( pStates[SDL_SCANCODE_LEFT] )
		{
			// MOVING LEFT
			m_IsMoving = true;
			m_IsMovingRight = false;
			CheckCrawling();
			if (m_TopActionState != TopActionState::crawling)
			{
				m_ActTopAnimation = Animations::movingLeft;
				m_ActBotAnimation = Animations::movingLeft;
			}
			else
			{
				m_ActTopAnimation = Animations::crawlingLeft;
				m_ActBotAnimation = Animations::crawlingLeft;
			}

			if ( ( m_BotActionState == BotActionState::jumping ) && m_IsOnGround)
			{
				// Change animation after jumping when ONGROUND
				m_TopActionState = TopActionState::iddle;
				m_BotActionState = BotActionState::iddle;
				m_ActTopAnimation = Animations::iddle;
				m_ActBotAnimation = Animations::iddle;
			}

			if ( pStates[SDL_SCANCODE_C] )
			{
				// JUMP + MOVE LEFT
				if ( m_IsOnGround )
				{
					m_TopActionState = TopActionState::jumping;
					m_BotActionState = BotActionState::jumping;
					m_ActTopAnimation = Animations::jumping;
					m_ActBotAnimation = Animations::jumping;
					m_Velocity.y = m_JumpSpeed;

				}
			}
		}
		else
		{
			// NO MOVEMENT
			if ( m_IsOnGround )
			{
				m_Velocity.x = 0;
			}
		}
	}
	


	// Check if shooting
	CheckShooting();

	// Check if an Action State has changed
	CheckPreviousState();

}


// Check if shooting and which type of shooting is doing
void Avatar::CheckShooting()
{
	if ( m_IsShooting )
	{
		if ( m_BotActionState == BotActionState::crawling )
		{
			m_BotActionState = BotActionState::shooting;
			m_ActBotAnimation = Animations::crawlingShooting;
			m_ActTopAnimation = Animations::crawlingShooting;
		}
		else
		{
			if ( m_TopActionState == TopActionState::pointingUp )
			{
				m_TopActionState = TopActionState::shootingUp;
				m_ActTopAnimation = Animations::shootingUp;
			}
			else
			{
				m_TopActionState = TopActionState::shooting;
				m_ActTopAnimation = Animations::shooting;
			}
		}

	}
}

// Check if Sprite has changed from the previous one. 
void Avatar::CheckPreviousState( )
{
	// TOP SPRITE CHECK
	if ( m_ActTopAnimation != m_PrevTopAnimation )
	{
		if ( ( m_PrevTopAnimation == Animations::shooting ) && ( m_ActTopAnimation == Animations::shootingUp ) )
		{
			m_TopActionState = TopActionState::pointingUp;
			m_IsShooting = false;
		}
		else
		{
			if ( (m_PrevTopAnimation == Animations::shootingUp ) && ( m_ActTopAnimation == Animations::shooting ) )
			{
				
				m_TopActionState = TopActionState::iddle;
				m_IsShooting = false;
			}
		}

		m_PrevTopAnimation = m_ActTopAnimation;
		m_TopSpriteChanged = true;
	}

	// BOTTOM SPRITE CHECK
	if ( m_ActBotAnimation != m_PrevBotAnimation )
	{
		m_PrevBotAnimation = m_ActBotAnimation;
		m_BotSpriteChanged = true;
	}

}

// Check if Avatar Crawling, reduce speed if so
void Avatar::CheckCrawling()
{   
	
	if (m_BotActionState == BotActionState::crawling)
	{
		// Speed is reduced when crawling

		if (!m_IsShooting)
		{
			if (m_IsMovingRight)
			{
				m_Velocity.x = m_SlowSpeed;
			}
			else
			{
				m_Velocity.x = -m_SlowSpeed;
			}
		}
		else
		{
			m_Velocity.x = 0.f;
		}
	}
	else
	{
		if (m_IsMovingRight)
		{
			m_Velocity.x = m_NormalSpeed;
		}
		else
		{
			m_Velocity.x = -m_NormalSpeed;
		}
	}
	
}

void Avatar::Move( float elapsedSec )
{
	// Check if we leave camera boundaries
	CheckCameraBoundaries();

	// Legs
	m_pBottomSprite->SetLeftDstRect( m_pBottomSprite->GetDstRect().left + m_Velocity.x * elapsedSec );
	m_pBottomSprite->SetBottomDstRect( m_pBottomSprite->GetDstRect().bottom + m_Velocity.y * elapsedSec );

	// Body
	m_pTopSprite->SetLeftDstRect(m_pTopSprite->GetDstRect().left + m_Velocity.x * elapsedSec);
	CorrectTopSprite();
	
}

// Check if we exit the camera boundaries. Stop movement if true
void Avatar::CheckCameraBoundaries()
{
	// Left boundarie
	if (!m_IsMovingRight && (m_pBottomSprite->GetDstRect().left - m_pBottomSprite->GetFrameWidth() < m_CameraPos.left))
	{
		m_Velocity.x = 0.f;

	}

	// Right boundarie
	if (m_IsMovingRight && (m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetFrameWidth() >= 
		m_CameraPos.left + m_CameraPos.width - m_pBottomSprite->GetFrameWidth()) )
	{
		m_Velocity.x = 0.f;
	}
}

void Avatar::Shoot()
{
	if (m_BotActionState != BotActionState::crawling && !m_IsDead )
	{
		m_pSoundManager->PlaySoundEffect(m_pPistolFire, 0);

		m_IsShooting = true;

		m_pBulletManager->ActivateBullet();

		m_pTopSprite->ResetAnimationFinish(false);
		ResetSprite(m_pTopSprite, true);
	}
	/*
	else
	{
		m_pBottomSprite->ResetAnimationFinish(false);
		ResetSprite(m_pBottomSprite, false);
	}
	*/
}

// Correct the top sprite so it draws it correctly
void Avatar::CorrectTopSprite()
{
	m_pTopSprite->SetBottomDstRect(m_pBottomSprite->GetDstRect().bottom + m_pBottomSprite->GetFrameHeight() + m_Offset);
}

// Gravity
void Avatar::AvatarFalling( float elapsedSec )
{
	m_Velocity.y += m_Acceleration.y * elapsedSec;

}

// Avatar has been hit = Death
void Avatar::Hit()
{
	
	if (!m_IsDead && !m_IsImmortal)
	{
		if (m_NrLifes > -1 )
		{
			m_NrLifes--;
		}

		ResetSprite(m_pBottomSprite, false);
		m_IsDead = true;
		m_Velocity = Vector2f{ 0.f, 0.f };

		m_TopActionState = TopActionState::death;
		m_BotActionState = BotActionState::death;
		m_ActBotAnimation = Animations::death;
		m_ActTopAnimation = Animations::death;

		m_pSoundManager->PlaySoundEffect(m_pDeathSound, 0);

	}
}

// Respaw the player position according with the camera Pos (if there are lifes left)
void Avatar::Respawn(float elapsedSec)
{
	m_SecondsRespawn += elapsedSec;

	if (m_SecondsRespawn >= m_MaxTimeRespawn)
	{
		// Check if still lifes left
		if (m_NrLifes > -1)
		{
			// Respawn player
			ResetSprite(m_pBottomSprite, false);
			m_SecondsRespawn = 0.f;
			m_IsDead = false;

			m_TopActionState = TopActionState::iddle;
			m_BotActionState = BotActionState::iddle;
			m_ActBotAnimation = Animations::iddle;
			m_ActTopAnimation = Animations::iddle;

			// Spawn at the middle of the window
			Point2f respawnPos{ m_CameraPos.left + m_CameraPos.width / 2, m_pBottomSprite->GetDstRect().bottom };
			m_pBottomSprite->SetLeftDstRect(respawnPos.x);
			m_pTopSprite->SetLeftDstRect(respawnPos.x);

			m_IsImmortal = true;
		}
		else
		{
			// No more lifes. Game Over
			m_AreLifesLeft = false;
		}
		

	}


}

// Avoid all damage for a few seconds after respawning
void Avatar::AvoidDamage(float elapsedSec)
{
	m_SecondsImmortal += elapsedSec;

	if (m_SecondsImmortal >= m_MaxTimeImmortal)
	{
		m_IsImmortal = false;
		m_CountNrFrames = false;
		m_SecondsImmortal = 0.f;
	}

	DoBlink(elapsedSec);
		
}


// CHeck the position of the avatar and change the stage of the game if he has reached
// to one of the stages position
void Avatar::CheckGameState()
{
	
	if (!m_IsFirstHeliFightStart)
	{
		if ((m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width) > g_Stage2Pos * g_Scale)
		{
			// Second Stage of the GAME --> First fight with the helicopter
			m_GameState = GameStage::firstHeliFight;
			m_IsFirstHeliFightStart = true;
			m_IsStageChanged = true;
			
		}
	}
	else
	{
		if (!m_IsSecondHeliFightStart)
		{
			if ((m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width) > g_Stage3Pos * g_Scale )
			{
				// Third Stage of the GAME --> Second fight with the helicopter
				m_GameState = GameStage::secondHeliFight;
				m_IsSecondHeliFightStart = true;
				m_IsStageChanged = true;
			}
		}
		else
		{ 
			if (!m_IsBossFightStart)
			{
				if ((m_pBottomSprite->GetDstRect().left + m_pBottomSprite->GetDstRect().width) > g_Stage4Pos * g_Scale )
				{

					m_GameState = GameStage::boss;
					m_IsBossFightStart = true;
					m_IsStageChanged = true;

				}
			}
		
		}
	}
	
	
}

// Stop drawing the sprite every 15 frames
void Avatar::DoBlink(float elapsedSec)
{
	if (m_CountNrFrames >= 20)
	{
		if (m_IsBlink)
		{
			m_IsBlink = false;
		}
		else
		{
			m_IsBlink = true;
		}

		m_CountNrFrames = 0;
	}
	else
	{
		m_CountNrFrames++;
	}
}

// Check what item has been grabbed
void Avatar::GrabItem(Item::ItemType itemType)
{
	switch (itemType)
	{
	case Item::ItemType::ammo:
		// Give ammo to avatar
		break;
	}
}

const bool Avatar::GetIsMovingRight() const
{
	return m_IsMovingRight;
}

const Avatar::Animations Avatar::GetActiveAnimation() const
{
	return m_ActTopAnimation;
}

BulletManager* Avatar::GetBullets()
{
	return m_pBulletManager;
}

// Get hitbox of the avatar for collisions
Rectf Avatar::GetHitBox() const
{
	return m_pTopSprite->GetHitBox();
}

int Avatar::GetNrLifes() const
{
	return m_NrLifes;
}

const bool Avatar::GetAvatarAlive() const
{
	return m_AreLifesLeft;
}

unsigned int Avatar::GetTotalPoints() const
{
	return m_pBulletManager->GetTotalPoints();
}

void Avatar::SetVerticesLevel(std::vector<Point2f> vertices)
{
	m_pBulletManager->SetVerticesLevel(vertices);
}

void Avatar::AddItemPoints(const unsigned int points)
{
	m_pBulletManager->AddPoints(points);
}
