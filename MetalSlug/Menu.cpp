#include "pch.h"
#include "Menu.h"
#include "Sprite.h"
#include "SoundManager.h"
#include <iostream>

Menu::Menu(const Point2f& windowSize, SoundManager* soundManager)
	: m_MenuState { State::startMenu }
	,m_pSoundManager { soundManager }
	, m_pMenuSong{ }
	, m_pNarratorVoice{ }
	, m_pMenuSound{ }
	, m_pBackground { }
	, m_pPushStart { }
	, m_pTitleBotPart{ }
	, m_pTitleTopPart{ }
	, m_pBulletCircle{ }
	, m_pBulletHole { }
	, m_StartPlaying { false }
	, m_StartLettersPos{ 270.f, 50.f }
	, m_WindowSize { windowSize }
	, m_TitleVel { 900.f }
	, m_DrawLetters { true }
	, m_FlickeringMaxTime{ 0.6f }
	, m_FlickeringSec { }
	, m_ScalingValue { 1.8f }
	, m_StartAnimationValue { 0.1f }
{
	Initialize();
}

Menu::~Menu()
{
	delete m_pBackground;
	delete m_pPushStart;
	delete m_pTitleBotPart;
	delete m_pTitleTopPart;
	delete m_pBulletCircle;
	delete m_pBulletHole;
}

void Menu::Initialize()
{
	// SOUNDS
	m_pMenuSong = m_pSoundManager->GetSound("Resources/Sounds/MenuSoundtrack.mp3");
	m_pNarratorVoice = m_pSoundManager->GetEffect("Resources/Sounds/Metalslug.wav");
	m_pMenuSound = m_pSoundManager->GetEffect("Resources/Sounds/MenuEffect.mp3");

	// SPRITES
	// Background Screen
	m_pBackground = new Sprite("Resources/sprites/HUD/startScreen.png");
	m_pBackground->UpdateValues(1, 1, 1, 1.f, m_pBackground->GetTexture()->GetWidth(), m_pBackground->GetTexture()->GetHeight(), m_pBackground->GetTexture()->GetHeight());
	m_pBackground->SetLeftDstRect(0.f);
	m_pBackground->SetBottomDstRect(0.f);

	// PUSH START letters
	m_pPushStart = new Sprite("Resources/sprites/HUD/PushStart2.png");
	m_pPushStart->UpdateValues(1, 1, 1, 1.f, m_pPushStart->GetTexture()->GetWidth(), m_pPushStart->GetTexture()->GetHeight(), m_pPushStart->GetTexture()->GetHeight(), 3.5f);
	m_pPushStart->SetLeftDstRect(m_StartLettersPos.x);
	m_pPushStart->SetBottomDstRect(m_StartLettersPos.y);

	// TITLE TEXT TOP PART
	m_pTitleTopPart = new Sprite("Resources/sprites/HUD/titleTopPart.png");
	m_pTitleTopPart->UpdateValues(1, 1, 1, 1.f, m_pTitleTopPart->GetTexture()->GetWidth(), m_pTitleTopPart->GetTexture()->GetHeight(), m_pTitleTopPart->GetTexture()->GetHeight());
	m_pTitleTopPart->SetLeftDstRect(m_WindowSize.x);
	m_pTitleTopPart->SetBottomDstRect((m_WindowSize.y / 2.f));

	// TITLE TEXT BOTTOM PART
	m_pTitleBotPart = new Sprite("Resources/sprites/HUD/titleBottomPart.png");
	m_pTitleBotPart->UpdateValues(1, 1, 1, 1.f, m_pTitleBotPart->GetTexture()->GetWidth(), m_pTitleBotPart->GetTexture()->GetHeight(), m_pTitleBotPart->GetTexture()->GetHeight());
	m_pTitleBotPart->SetLeftDstRect(0 - (m_pTitleBotPart->GetFrameWidth() * g_Scale));
	m_pTitleBotPart->SetBottomDstRect((m_WindowSize.y / 2.f) - (m_pTitleBotPart->GetFrameHeight() * 2.1f));

	// Big Bullet Circle
	m_pBulletCircle = new Sprite("Resources/sprites/HUD/intro.png");
	m_pBulletCircle->UpdateValues(1, 1, 1, 1.f, m_pBulletCircle->GetTexture()->GetWidth(), m_pBulletCircle->GetTexture()->GetHeight(), m_pBulletCircle->GetTexture()->GetHeight());
	m_pBulletCircle->SetLeftDstRect( ( m_WindowSize.x / 2.f) - (m_pBulletCircle->GetFrameWidth() * g_Scale) / 2.f  + 100.f);
	m_pBulletCircle->SetBottomDstRect((m_WindowSize.y / 2.f) - (m_pBulletCircle->GetFrameHeight() * g_Scale) / 2.f + 100.f);

	m_pBulletHole = new Sprite("Resources/sprites/HUD/bulletHole.png");
	m_pBulletHole->UpdateValues(1, 1, 1, 1.f, m_pBulletHole->GetTexture()->GetWidth(), m_pBulletHole->GetTexture()->GetHeight(), m_pBulletHole->GetTexture()->GetHeight());
	m_pBulletHole->SetLeftDstRect((m_WindowSize.x / 2.f) - (m_pBulletHole->GetFrameWidth() * g_Scale) / 2.f );
	m_pBulletHole->SetBottomDstRect((m_WindowSize.y / 2.f) - (m_pBulletHole->GetFrameHeight() * g_Scale) / 2.f);

}

void Menu::Draw() const
{
	m_pBackground->Draw();

	switch (m_MenuState)
	{
	case Menu::State::startMenu:
		
		glPushMatrix();

			m_pBulletCircle->ScaleSprite(m_ScalingValue);
			m_pBulletCircle->Draw();

		glPopMatrix();
		break;

	case Menu::State::startAnimation:

		m_pBulletHole->Draw();
		m_pTitleTopPart->Draw();
		m_pTitleBotPart->Draw();
		break;

	case Menu::State::waitingToPlay:

		m_pBulletHole->Draw();
		m_pTitleTopPart->Draw();
		m_pTitleBotPart->Draw();

		if (m_DrawLetters)
		{
			m_pPushStart->Draw();
		}
		break;
	}


	

	
}

void Menu::Update(float elapsedSec, Game::GameState& gameState)
{
	switch (m_MenuState)
	{
	case Menu::State::startMenu:

		if (m_ScalingValue > 0.05f)
		{
			// Scale down the circle sprite
			m_ScalingValue -= 2.f * elapsedSec;
		}
		else
		{
			m_pSoundManager->PlaySoundEffect(m_pMenuSound, 0);
			m_MenuState = State::startAnimation;
		}
		
		break;

	case Menu::State::startAnimation:

		UpdateMenuAnimation(elapsedSec);
		break;

	case Menu::State::waitingToPlay:

		UpdateFlicker(elapsedSec);
		if (m_StartPlaying)
		{
			// Start playing the game
			gameState = Game::GameState::playing;
		}
		break;
	}
	
	
}

void Menu::UpdateMenuAnimation(float elapsedSec)
{
	// Move title letters into the correct pos
	if (m_pTitleTopPart->GetDstRect().left + (m_pTitleTopPart->GetFrameWidth() * g_Scale / 2.f) > m_WindowSize.x / 2.f)
	{
		m_pTitleTopPart->SetLeftDstRect(m_pTitleTopPart->GetDstRect().left - (m_TitleVel * elapsedSec));
		m_pTitleBotPart->SetLeftDstRect(m_pTitleBotPart->GetDstRect().left + (m_TitleVel * elapsedSec));
	}
	else
	{
		// Title letters in position
		m_pSoundManager->PlaySoundEffect(m_pNarratorVoice, 0);
		m_pSoundManager->PlaySong(m_pMenuSong, true);
		m_MenuState = State::waitingToPlay;
	}
	
}

// Flicker effect for the Start letters
void Menu::UpdateFlicker(float elapsedSec)
{
	if (m_FlickeringSec >= m_FlickeringMaxTime)
	{
		if (m_DrawLetters)
		{
			m_DrawLetters = false;
		}
		else
		{
			m_DrawLetters = true;
		}

		m_FlickeringSec = 0.f;
	}
	else
	{
		m_FlickeringSec += elapsedSec;
	}

}

void Menu::SetPlaying(bool start)
{
	if (m_MenuState == State::waitingToPlay)
	{
		// Stop the menu song
		m_pMenuSong->Stop();

		m_StartPlaying = start;
	}
	
}

bool Menu::GetStartPlaying() const
{
	return m_StartPlaying;
}