#include "pch.h"
#include "Pause.h"
#include "Sprite.h"
#include "utils.h"
#include <iostream>


Pause::Pause(const Point2f& windowSize, SoundManager* soundManager)
	: m_pMainPauseBackground { }
	, m_pControlsBackground{ }
	, m_pSettingsBackground{ }
	, m_pMusicNr{ }
	, m_pEffectsNr{ }
	, m_pBackTexts{ }
	, m_pMainPauseTexts { }
	, m_pVolumeArrows{ }
	, m_pMusicArrows{ }
	, m_pMusicArrowsR{ }
	, m_pEffectsArrows{ }
	, m_pEffectsArrowsR{ }
	, m_ClosePauseMenu { false }
	, m_VolumeChangeValue { 10 }
	, m_pSoundManager { soundManager }   // Works because we are copying pointers not the object itself
	, m_PauseState { PauseState::mainPause }
	
{
	Initialize(windowSize);
}


Pause::~Pause()
{
	delete m_pMainPauseBackground;
	delete m_pControlsBackground;
	delete m_pSettingsBackground;

	delete m_pBackTexts.first;
	delete m_pBackTexts.second;

	delete m_pMusicNr;
	delete m_pEffectsNr;

	for (std::pair<Sprite*, Sprite*> spr : m_pMainPauseTexts)
	{
		delete spr.first;
		delete spr.second;
	}

	for (std::pair<Sprite*, Sprite*> spr : m_pVolumeArrows)
	{
		delete spr.first;
		delete spr.second;
	}


}


void Pause::Initialize(const Point2f& windowSize)
{
	
	InitBackgrounds(windowSize);
	InitTexts(windowSize);
	InitArrows();
	InitVolumeValues();
	
}

void Pause::InitBackgrounds(const Point2f& windowSize)
{
	// BACKGROUNDS SPRITES FOR THE PAUSE MENU
	
	m_pMainPauseBackground = new Sprite("Resources/sprites/HUD/Pause/pauseMenu.png");
	m_pMainPauseBackground->UpdateValues(1, 1, 1, 1.f, m_pMainPauseBackground->GetTexture()->GetWidth(), m_pMainPauseBackground->GetTexture()->GetHeight(), m_pMainPauseBackground->GetTexture()->GetHeight());
	m_pMainPauseBackground->SetLeftDstRect((windowSize.x / 2.f) - (m_pMainPauseBackground->GetFrameWidth() * g_Scale) / 2.f);
	m_pMainPauseBackground->SetBottomDstRect((windowSize.y / 2.f) - (m_pMainPauseBackground->GetFrameHeight() * g_Scale) / 2.f);

	m_pControlsBackground = new Sprite("Resources/sprites/HUD/Pause/keyInfo.png");
	m_pControlsBackground->UpdateValues(1, 1, 1, 1.f, m_pControlsBackground->GetTexture()->GetWidth(), m_pControlsBackground->GetTexture()->GetHeight(), m_pControlsBackground->GetTexture()->GetHeight());
	m_pControlsBackground->SetLeftDstRect((windowSize.x / 2.f) - (m_pControlsBackground->GetFrameWidth() * g_Scale) / 2.f);
	m_pControlsBackground->SetBottomDstRect((windowSize.y / 2.f) - (m_pControlsBackground->GetFrameHeight() * g_Scale) / 2.f);

	m_pSettingsBackground = new Sprite("Resources/sprites/HUD/Pause/settingsMenu.png");
	m_pSettingsBackground->UpdateValues(1, 1, 1, 1.f, m_pSettingsBackground->GetTexture()->GetWidth(), m_pSettingsBackground->GetTexture()->GetHeight(), m_pSettingsBackground->GetTexture()->GetHeight());
	m_pSettingsBackground->SetLeftDstRect((windowSize.x / 2.f) - (m_pSettingsBackground->GetFrameWidth() * g_Scale) / 2.f);
	m_pSettingsBackground->SetBottomDstRect((windowSize.y / 2.f) - (m_pSettingsBackground->GetFrameHeight() * g_Scale) / 2.f);

}

void Pause::InitTexts(const Point2f& windowSize)
{
	// TEXTS
	// CONTINUE TEXT 
	Sprite* pContinue{ new Sprite("Resources/sprites/HUD/Pause/continue.png") };
	pContinue->UpdateValues(1, 1, 1, 1.f, pContinue->GetTexture()->GetWidth(), pContinue->GetTexture()->GetHeight(), pContinue->GetTexture()->GetHeight());
	pContinue->SetLeftDstRect((windowSize.x / 2.f) - (pContinue->GetFrameWidth() * g_Scale) / 2.f);
	pContinue->SetBottomDstRect((windowSize.y / 2.f) + (pContinue->GetFrameHeight() * g_Scale));

	// CONTINUE TEXT (Colored)
	Sprite* pContinueColored{ new Sprite("Resources/sprites/HUD/Pause/continueColor.png") };
	pContinueColored->UpdateValues(1, 1, 1, 1.f, pContinueColored->GetTexture()->GetWidth(), pContinueColored->GetTexture()->GetHeight(), pContinueColored->GetTexture()->GetHeight());
	pContinueColored->SetLeftDstRect(pContinue->GetDstRect().left);
	pContinueColored->SetBottomDstRect(pContinue->GetDstRect().bottom);

	// First sprite will be without color and second one with color
	m_pMainPauseTexts.push_back(std::make_pair(pContinue, pContinueColored));
	const float textSpacing{ 10.f };

	// SETTINGS TEXT
	Sprite* pSettings{ new Sprite("Resources/sprites/HUD/Pause/settings.png") };
	pSettings->UpdateValues(1, 1, 1, 1.f, pSettings->GetTexture()->GetWidth(), pSettings->GetTexture()->GetHeight(), pSettings->GetTexture()->GetHeight());
	pSettings->SetLeftDstRect(pContinue->GetDstRect().left);
	pSettings->SetBottomDstRect(pContinueColored->GetDstRect().bottom - (pSettings->GetFrameHeight() * g_Scale) - textSpacing);

	// SETTINGS TEXT (COLORED)
	Sprite* pSettingsColored{ new Sprite("Resources/sprites/HUD/Pause/settingsColor.png") };
	pSettingsColored->UpdateValues(1, 1, 1, 1.f, pSettingsColored->GetTexture()->GetWidth(), pSettingsColored->GetTexture()->GetHeight(), pSettingsColored->GetTexture()->GetHeight());
	pSettingsColored->SetLeftDstRect(pContinue->GetDstRect().left);
	pSettingsColored->SetBottomDstRect(pSettings->GetDstRect().bottom);

	m_pMainPauseTexts.push_back(std::make_pair(pSettings, pSettingsColored));

	// CONTROLS TEXT
	Sprite* pControls{ new Sprite("Resources/sprites/HUD/Pause/controls.png") };
	pControls->UpdateValues(1, 1, 1, 1.f, pControls->GetTexture()->GetWidth(), pControls->GetTexture()->GetHeight(), pControls->GetTexture()->GetHeight());
	pControls->SetLeftDstRect(pContinue->GetDstRect().left);
	pControls->SetBottomDstRect(pSettings->GetDstRect().bottom - (pControls->GetFrameHeight() * g_Scale) - textSpacing);

	// CONTROLS TEXT (COLORED)
	Sprite* pControlsColored{ new Sprite("Resources/sprites/HUD/Pause/controlsColor.png") };
	pControlsColored->UpdateValues(1, 1, 1, 1.f, pControlsColored->GetTexture()->GetWidth(), pControlsColored->GetTexture()->GetHeight(), pControlsColored->GetTexture()->GetHeight());
	pControlsColored->SetLeftDstRect(pContinue->GetDstRect().left);
	pControlsColored->SetBottomDstRect(pControls->GetDstRect().bottom);

	m_pMainPauseTexts.push_back(std::make_pair(pControls, pControlsColored));

	// QUIT TEXT
	Sprite* pQuit{ new Sprite("Resources/sprites/HUD/Pause/quit.png") };
	pQuit->UpdateValues(1, 1, 1, 1.f, pQuit->GetTexture()->GetWidth(), pQuit->GetTexture()->GetHeight(), pQuit->GetTexture()->GetHeight());
	pQuit->SetLeftDstRect(pContinue->GetDstRect().left);
	pQuit->SetBottomDstRect(pControls->GetDstRect().bottom - (pQuit->GetFrameHeight() * g_Scale) - textSpacing);

	// QUIT TEXT (COLORED)
	Sprite* pQuitColored{ new Sprite("Resources/sprites/HUD/Pause/quitColor.png") };
	pQuitColored->UpdateValues(1, 1, 1, 1.f, pQuitColored->GetTexture()->GetWidth(), pQuitColored->GetTexture()->GetHeight(), pQuitColored->GetTexture()->GetHeight());
	pQuitColored->SetLeftDstRect(pContinue->GetDstRect().left);
	pQuitColored->SetBottomDstRect(pQuit->GetDstRect().bottom);

	m_pMainPauseTexts.push_back(std::make_pair(pQuit, pQuitColored));

	// BACK TEXT
	m_pBackTexts.first = new Sprite("Resources/sprites/HUD/pause/back.png");
	m_pBackTexts.first->UpdateValues(1, 1, 1, 1.f, m_pBackTexts.first->GetTexture()->GetWidth(), m_pBackTexts.first->GetTexture()->GetHeight(), m_pBackTexts.first->GetTexture()->GetHeight());
	m_pBackTexts.first->SetLeftDstRect((windowSize.x / 2.f) - (m_pBackTexts.first->GetFrameWidth() * g_Scale) / 2.f);
	m_pBackTexts.first->SetBottomDstRect(m_pControlsBackground->GetDstRect().bottom + (m_pBackTexts.first->GetFrameHeight()));

	// BACK TEXT (Colored)
	m_pBackTexts.second = new Sprite("Resources/sprites/HUD/pause/backColor.png");
	m_pBackTexts.second->UpdateValues(1, 1, 1, 1.f, m_pBackTexts.second->GetTexture()->GetWidth(), m_pBackTexts.second->GetTexture()->GetHeight(), m_pBackTexts.second->GetTexture()->GetHeight());
	m_pBackTexts.second->SetLeftDstRect(m_pBackTexts.first->GetDstRect().left);
	m_pBackTexts.second->SetBottomDstRect(m_pBackTexts.first->GetDstRect().bottom);

}

void Pause::InitArrows()
{
	// MUSIC ARROWS
	m_pMusicArrows.first = new Sprite("Resources/sprites/HUD/Pause/arrow.png");
	m_pMusicArrows.first->UpdateValues(1, 1, 1, 1.f, m_pMusicArrows.first->GetTexture()->GetWidth(), m_pMusicArrows.first->GetTexture()->GetHeight(), m_pMusicArrows.first->GetTexture()->GetHeight());
	m_pMusicArrows.first->SetLeftDstRect((m_pSettingsBackground->GetDstRect().left + m_pSettingsBackground->GetFrameWidth()) + (m_pMusicArrows.first->GetFrameWidth()) * 10.f);
	m_pMusicArrows.first->SetBottomDstRect((m_pSettingsBackground->GetDstRect().bottom + m_pSettingsBackground->GetFrameHeight()) + m_pMusicArrows.first->GetFrameHeight() * 3.2f);

	// (Colored)
	m_pMusicArrows.second = new Sprite("Resources/sprites/HUD/Pause/arrowColor.png");
	m_pMusicArrows.second->UpdateValues(1, 1, 1, 1.f, m_pMusicArrows.second->GetTexture()->GetWidth(), m_pMusicArrows.second->GetTexture()->GetHeight(), m_pMusicArrows.second->GetTexture()->GetHeight());
	m_pMusicArrows.second->SetLeftDstRect(m_pMusicArrows.first->GetDstRect().left);
	m_pMusicArrows.second->SetBottomDstRect(m_pMusicArrows.first->GetDstRect().bottom);

	m_pVolumeArrows.push_back(m_pMusicArrows);

	// RIGHT ARROW
	m_pMusicArrowsR.first = new Sprite("Resources/sprites/HUD/Pause/arrowR.png");
	m_pMusicArrowsR.first->UpdateValues(1, 1, 1, 1.f, m_pMusicArrowsR.first->GetTexture()->GetWidth(), m_pMusicArrowsR.first->GetTexture()->GetHeight(), m_pMusicArrowsR.first->GetTexture()->GetHeight());
	m_pMusicArrowsR.first->SetLeftDstRect((m_pSettingsBackground->GetDstRect().left + m_pSettingsBackground->GetFrameWidth()) + (m_pMusicArrowsR.first->GetFrameWidth()) * 18.f);
	m_pMusicArrowsR.first->SetBottomDstRect((m_pSettingsBackground->GetDstRect().bottom + m_pSettingsBackground->GetFrameHeight()) + m_pMusicArrowsR.first->GetFrameHeight() * 3.2f);

	m_pMusicArrowsR.second = new Sprite("Resources/sprites/HUD/Pause/arrowColorR.png");
	m_pMusicArrowsR.second->UpdateValues(1, 1, 1, 1.f, m_pMusicArrowsR.second->GetTexture()->GetWidth(), m_pMusicArrowsR.second->GetTexture()->GetHeight(), m_pMusicArrowsR.second->GetTexture()->GetHeight());
	m_pMusicArrowsR.second->SetLeftDstRect(m_pMusicArrowsR.first->GetDstRect().left);
	m_pMusicArrowsR.second->SetBottomDstRect(m_pMusicArrowsR.first->GetDstRect().bottom);

	m_pVolumeArrows.push_back(m_pMusicArrowsR);

	// EFFECTS ARROWS
	m_pEffectsArrows.first = new Sprite("Resources/sprites/HUD/Pause/arrow.png");
	m_pEffectsArrows.first->UpdateValues(1, 1, 1, 1.f, m_pEffectsArrows.first->GetTexture()->GetWidth(), m_pEffectsArrows.first->GetTexture()->GetHeight(), m_pEffectsArrows.first->GetTexture()->GetHeight());
	m_pEffectsArrows.first->SetLeftDstRect(m_pMusicArrows.first->GetDstRect().left);
	m_pEffectsArrows.first->SetBottomDstRect((m_pSettingsBackground->GetDstRect().bottom + m_pSettingsBackground->GetFrameHeight()) - m_pEffectsArrows.first->GetFrameHeight() * 1.f);

	// (Colored)
	m_pEffectsArrows.second = new Sprite("Resources/sprites/HUD/Pause/arrowColor.png");
	m_pEffectsArrows.second->UpdateValues(1, 1, 1, 1.f, m_pEffectsArrows.second->GetTexture()->GetWidth(), m_pEffectsArrows.second->GetTexture()->GetHeight(), m_pEffectsArrows.second->GetTexture()->GetHeight());
	m_pEffectsArrows.second->SetLeftDstRect(m_pEffectsArrows.first->GetDstRect().left);
	m_pEffectsArrows.second->SetBottomDstRect(m_pEffectsArrows.first->GetDstRect().bottom);

	m_pVolumeArrows.push_back(m_pEffectsArrows);

	// RIGHT ARROW
	m_pEffectsArrowsR.first = new Sprite("Resources/sprites/HUD/Pause/arrowR.png");
	m_pEffectsArrowsR.first->UpdateValues(1, 1, 1, 1.f, m_pEffectsArrowsR.first->GetTexture()->GetWidth(), m_pEffectsArrowsR.first->GetTexture()->GetHeight(), m_pEffectsArrowsR.first->GetTexture()->GetHeight());
	m_pEffectsArrowsR.first->SetLeftDstRect(m_pMusicArrowsR.first->GetDstRect().left);
	m_pEffectsArrowsR.first->SetBottomDstRect((m_pSettingsBackground->GetDstRect().bottom + m_pSettingsBackground->GetFrameHeight()) - m_pEffectsArrowsR.first->GetFrameHeight() * 1.f);

	// (Colored)
	m_pEffectsArrowsR.second = new Sprite("Resources/sprites/HUD/Pause/arrowColorR.png");
	m_pEffectsArrowsR.second->UpdateValues(1, 1, 1, 1.f, m_pEffectsArrowsR.second->GetTexture()->GetWidth(), m_pEffectsArrowsR.second->GetTexture()->GetHeight(), m_pEffectsArrowsR.second->GetTexture()->GetHeight());
	m_pEffectsArrowsR.second->SetLeftDstRect(m_pEffectsArrowsR.first->GetDstRect().left);
	m_pEffectsArrowsR.second->SetBottomDstRect(m_pEffectsArrowsR.first->GetDstRect().bottom);

	m_pVolumeArrows.push_back(m_pEffectsArrowsR);
}

void Pause::InitVolumeValues()
{
	// MUSIC Values
	m_pMusicNr = new Sprite("Resources/sprites/HUD/Pause/volumeNrs.png");
	m_pMusicNr->UpdateValues(10, 1, 10, 1.f, m_pMusicNr->GetTexture()->GetWidth() / 10.f, m_pMusicNr->GetTexture()->GetHeight(), m_pMusicNr->GetTexture()->GetHeight());
	m_pMusicNr->SetLeftDstRect(m_pMusicArrows.first->GetDstRect().left + (m_pMusicNr->GetFrameWidth() * 4.5f));
	m_pMusicNr->SetBottomDstRect(m_pMusicArrows.first->GetDstRect().bottom);
	m_pMusicNr->SetActFrame(9);

	// Effect Values
	m_pEffectsNr = new Sprite("Resources/sprites/HUD/Pause/volumeNrs.png");
	m_pEffectsNr->UpdateValues(10, 1, 10, 1.f, m_pEffectsNr->GetTexture()->GetWidth() / 10.f, m_pEffectsNr->GetTexture()->GetHeight(), m_pEffectsNr->GetTexture()->GetHeight());
	m_pEffectsNr->SetLeftDstRect(m_pEffectsArrows.first->GetDstRect().left + (m_pEffectsNr->GetFrameWidth() * 4.5f));
	m_pEffectsNr->SetBottomDstRect(m_pEffectsArrows.first->GetDstRect().bottom);
	m_pEffectsNr->SetActFrame(9);
}

void Pause::Draw() const
{
	switch (m_PauseState)
	{
	case Pause::PauseState::mainPause:

		DrawMainPause();
		break;

	case Pause::PauseState::settings:

		DrawSettings();
		break;

	case Pause::PauseState::controls:

		DrawControls();
		break;

	}
	
}

void Pause::DrawMainPause() const
{
	m_pMainPauseBackground->Draw();

	for (int i{ 0 }; i < m_pMainPauseTexts.size(); i++)
	{
		// Only those that are activated will be drawn
		m_pMainPauseTexts.at(i).first->Draw();				// Better at than [] to access elements
		m_pMainPauseTexts.at(i).second->Draw();
	}
}

void Pause::DrawSettings() const
{
	m_pSettingsBackground->Draw();

	m_pBackTexts.first->Draw();
	m_pBackTexts.second->Draw();

	for (int i{ 0 }; i < m_pVolumeArrows.size(); i++)
	{
		m_pVolumeArrows.at(i).first->Draw();
		m_pVolumeArrows.at(i).second->Draw();
	}

	m_pMusicNr->Draw();
	m_pEffectsNr->Draw();
}

void Pause::DrawControls() const
{
	m_pControlsBackground->Draw();
	m_pBackTexts.first->Draw();
	m_pBackTexts.second->Draw();
}


void Pause::Update(const Point2f& mousePos)
{
	switch (m_PauseState)
	{
	case Pause::PauseState::mainPause:

		UpdateMainPause(mousePos);
		break;

	case Pause::PauseState::settings:

		UpdateSettings(mousePos);
		break;

	case Pause::PauseState::controls:

		UpdateControls(mousePos);
		break;

	}
	
}

void Pause::UpdateMainPause(const Point2f& mousePos)
{
	for (int i{ 0 }; i < m_pMainPauseTexts.size(); i++)
	{
		HighlightSprite(mousePos, m_pMainPauseTexts.at(i));
	}
}

void Pause::UpdateSettings(const Point2f& mousePos)
{
	m_pBackTexts.first->SetBottomDstRect(m_pSettingsBackground->GetDstRect().bottom + (m_pBackTexts.first->GetFrameHeight()));
	m_pBackTexts.second->SetBottomDstRect(m_pBackTexts.first->GetDstRect().bottom);
	HighlightSprite(mousePos, m_pBackTexts);

	for (int i{ 0 }; i < m_pVolumeArrows.size(); i++)
	{
		HighlightSprite(mousePos, m_pVolumeArrows.at(i) );
	}

	
	CalculateVolumeValue();
}

void Pause::CalculateVolumeValue()
{
	int musicVolume{ m_pSoundManager->GetMusicVolume() };
	m_pMusicNr->SetActFrame(musicVolume / m_VolumeChangeValue);
	m_pMusicNr->UpdateLeftSrcRect();

	int effectsVolume{ m_pSoundManager->GetEffectsVolume() };
	m_pEffectsNr->SetActFrame(effectsVolume / m_VolumeChangeValue);
	m_pEffectsNr->UpdateLeftSrcRect();

}

void Pause::UpdateControls(const Point2f& mousePos)
{
	m_pBackTexts.first->SetBottomDstRect(m_pControlsBackground->GetDstRect().bottom + (m_pBackTexts.first->GetFrameHeight()));
	m_pBackTexts.second->SetBottomDstRect(m_pBackTexts.first->GetDstRect().bottom);
	HighlightSprite(mousePos, m_pBackTexts);

}

// Check wich selection the player chooses. It returns true if the player wants to close the game
void Pause::SelectOption(const Point2f& mousePos, bool& closeGame, bool& closePauseMenu)
{
	switch (m_PauseState)
	{
	case Pause::PauseState::mainPause:
		for (int i{ 0 }; i < m_pMainPauseTexts.size(); i++)
		{
			if (utils::IsPointInRect(mousePos, m_pMainPauseTexts.at(i).first->GetDstRect()))
			{
				switch (i)
				{
				case 0:
					// Continue playing (Close pause Menu)
					closePauseMenu = true;
					break;
				case 1:
					// Settings
					m_PauseState = PauseState::settings;
					break;
				case 2:
					// Show Controls
					m_PauseState = PauseState::controls;
					break;
				case 3:
					// Close Game
					closeGame = true;
					break;
				}
			}
		}
		break;
	case Pause::PauseState::settings:
		if (utils::IsPointInRect(mousePos, m_pBackTexts.first->GetDstRect()))
		{
			// Go back
			m_PauseState = PauseState::mainPause;
		}

		for (int i{}; i < m_pVolumeArrows.size(); i++)
		{
			if (utils::IsPointInRect(mousePos, m_pVolumeArrows.at(i).first->GetDstRect()))
			{
				switch (i)
				{
				case 0:
					m_pSoundManager->ChangeSoundtrackVolume(-m_VolumeChangeValue);
					break;

				case 1:
					m_pSoundManager->ChangeSoundtrackVolume(m_VolumeChangeValue);
					break;

				case 2:
					m_pSoundManager->ChangeEffectVolume(-m_VolumeChangeValue);
					break;

				case 3:
					m_pSoundManager->ChangeEffectVolume(m_VolumeChangeValue);
					break;

				}
			}
		}
		

		break;
	case Pause::PauseState::controls:
		if ( utils::IsPointInRect(mousePos, m_pBackTexts.first->GetDstRect()) )
		{
			// Go back
			m_PauseState = PauseState::mainPause;
		}
		break;
	}
	
}

// Highlight the sprite when mouse over
void Pause::HighlightSprite(const Point2f& mousePos, std::pair<Sprite*, Sprite*>& sprites)
{
	if (utils::IsPointInRect(mousePos, sprites.first->GetDstRect()))
	{
		// Draw colored text when mouse on it
		sprites.first->SetActive(false);
		sprites.second->SetActive(true);
	}
	else
	{
		// Draw text without color
		sprites.first->SetActive(true);
		sprites.second->SetActive(false);
	}
}