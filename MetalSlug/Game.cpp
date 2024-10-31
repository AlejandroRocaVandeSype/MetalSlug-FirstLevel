#include "pch.h"
#include "Game.h"
#include "Avatar.h"
#include "Helicopter.h"
#include "BulletManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "Soldier.h"
#include "Boss.h"
#include "HUD.h"
#include "Menu.h"
#include "Pause.h"
#include "FileReader.h"
#include <iostream>



Game::Game( const Window& window ) 
	:m_Window{ window }
	, m_pCamera{ }
	, m_pLevel{ }
	, m_pAvatar{ }
	, m_pGameObjectManager{ }
	, m_pSoundManager{ new SoundManager() }
	, m_pMenu { }
	, m_GameState { GameState::menu }
	, m_pGameOverScreen { }
	, m_pMissionStartSound { }
	, m_pMissionComplete{ }
	, m_pGameOverSong { }
	, m_pPause{ }
	, m_MousePos { }
	, m_CloseGame{ false }
	, m_ClosePauseMenu { false }
	, m_pBossSong { }
{
	InitMenu();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pLevel = new Level;

	InitAvatar();
	InitCamera();
	InitPause();			// Already load everything for the pause Menu
	CreateGameObjectsFromFile();
	InitHUD();

	InitLevelSounds();

	m_pHUD->ActivateGoTextAnimation();
	
	
}

void Game::InitAvatar()
{
	m_pAvatar = new Avatar(m_pSoundManager);
	m_pAvatar->SetVerticesLevel(m_pLevel->GetVertices());
}

void Game::InitCamera()
{
	m_pCamera = new Camera(m_Window.width, m_Window.height);
	m_pCamera->SetLevelBoundaries(m_pLevel->GetBoundaries());

}

void Game::InitHUD()
{
	// Bottom left and size of the window
	m_pHUD = new HUD(Point2f{ }, Point2f{ m_Window.width, m_Window.height } , m_pSoundManager, m_pGameObjectManager->GetTotalPrisoners()); 
}

void Game::InitMenu()
{
	m_pMenu = new Menu( Point2f{m_Window.width, m_Window.height}, m_pSoundManager );
}

// Init all the music and general sounds
void Game::InitLevelSounds()
{
	m_pLevelSong = m_pSoundManager->GetSound("Resources/Sounds/Soundtrack.mp3");
	m_pBossSong = m_pSoundManager->GetSound("Resources/Sounds/BossFight.mp3");
	m_pMissionStartSound = m_pSoundManager->GetEffect("Resources/Sounds/MissionStart.mp3");
	m_pMissionComplete = m_pSoundManager->GetEffect("Resources/Sounds/MissionCompleteVoice.mp3");

	m_pSoundManager->PlaySong(m_pLevelSong, true);
	m_pSoundManager->PlaySoundEffect(m_pMissionStartSound, 0);

}

void Game::InitGameOverState()
{
	// Avatar is dead. Game Over Screen
	m_GameState = GameState::gameOver;

	// Game Over screen
	m_pGameOverScreen = new Sprite("Resources/sprites/HUD/GameOver.png");
	m_pGameOverScreen->UpdateValues(1, 1, 1, 1.f, m_pGameOverScreen->GetTexture()->GetWidth(), m_pGameOverScreen->GetTexture()->GetHeight(), m_pGameOverScreen->GetTexture()->GetHeight());
	m_pGameOverScreen->SetLeftDstRect(0.f);
	m_pGameOverScreen->SetBottomDstRect(0.f);

	m_pGameOverSong = m_pSoundManager->GetSound("Resources/Sounds/GameOver.mp3");
	m_pSoundManager->PlaySong(m_pGameOverSong, false);
	
}

void Game::InitPause()
{
	m_pPause = new Pause ( Point2f{ m_Window.width, m_Window.height }, m_pSoundManager);
}

void  Game::CreateGameObjectsFromFile()
{
	m_pGameObjectManager = new GameObjectManager(m_pSoundManager);

	std::string filename{ "Resources/GameObjects.txt" };

	FileReader fileReader(filename);

	fileReader.CreateGameObjects(m_pGameObjectManager, '>');
	
}



void Game::Cleanup( )
{
	// Delete according in the state the player is
	switch (m_GameState)
	{
		case Game::GameState::menu:

			delete m_pMenu;
			break;

		case Game::GameState::playing:
		case Game::GameState::bossFight:
		case Game::GameState::missionComplete:

			delete m_pAvatar;
			delete m_pLevel;
			delete m_pCamera;
			delete m_pGameObjectManager;
			delete m_pHUD;
			delete m_pPause;
			break;

		case  Game::GameState::pause:

			delete m_pAvatar;
			delete m_pLevel;
			delete m_pCamera;
			delete m_pGameObjectManager;
			delete m_pHUD;
			delete m_pPause;
			break;

		case Game::GameState::gameOver:

			delete m_pAvatar;
			delete m_pLevel;
			delete m_pCamera;
			delete m_pGameObjectManager;
			delete m_pHUD;
			delete m_pGameOverScreen;
			delete m_pPause;
			break;
	}

	delete m_pSoundManager;
}

void Game::Update( float elapsedSec )
{
	switch (m_GameState)
	{
	case Game::GameState::menu:

		m_pMenu->Update(elapsedSec, m_GameState);

		if ( m_pMenu->GetStartPlaying() )
		{
			// Start playing --> Init all data needed
			Initialize();
			delete m_pMenu;
		}
		break;

	case Game::GameState::playing:

		UpdatePlaying(elapsedSec);
		break;

	case Game::GameState::bossFight:
		UpdatePlaying(elapsedSec);
		break;
	case Game::GameState::pause:

		m_pPause->Update(m_MousePos);
		break;

	case Game::GameState::gameOver:

		// Nothing to do here
		break;
	
	case Game::GameState::missionComplete:

		UpdateFinishLevel(elapsedSec);
		break;
	}
}

void Game::UpdatePlaying(float elapsedSec)
{
	m_pAvatar->Update(elapsedSec, m_pLevel, m_pCamera->GetCameraPos());

	m_pGameObjectManager->Update(elapsedSec, m_pAvatar, m_pLevel, m_pCamera->GetCameraPos());

	m_pAvatar->GetBullets()->CheckHitGameObjects(m_pGameObjectManager->GetGameObjects());

	m_pLevel->Update(elapsedSec, m_pAvatar->GetBotShape());

	m_pCamera->SetLevelBoundaries(m_pLevel->GetBoundaries());

	m_pHUD->Update( elapsedSec, m_pAvatar->GetNrLifes(), m_pAvatar->GetTotalPoints(), m_pGameObjectManager->GetTotalPrisoners() );

	if ( !m_pAvatar->GetAvatarAlive() || m_pHUD->GetTimeOver() )
	{
		InitGameOverState();
	}
	
	if ( m_GameState == GameState::playing && m_pAvatar->GetGameStage() == GameObject::GameStage::boss)
	{
		m_pLevelSong->Stop();
		m_pSoundManager->PlaySong(m_pBossSong, true);
		m_GameState = GameState::bossFight;
	}
	
	if (m_GameState == GameState::bossFight &&  m_pAvatar->GetGameStage() == GameObject::GameStage::end)
	{
		m_pBossSong->Stop();
		m_pSoundManager->PlaySoundEffect(m_pMissionComplete, 0);
		m_pHUD->SetEndLevel();
		m_GameState = GameState::missionComplete;
	}

}

void Game::UpdateFinishLevel(float elapsedSec)
{
	m_pGameObjectManager->Update(elapsedSec, m_pAvatar, m_pLevel, m_pCamera->GetCameraPos());

	m_pLevel->Update(elapsedSec, m_pAvatar->GetBotShape());

	m_pCamera->SetLevelBoundaries(m_pLevel->GetBoundaries());

	m_pHUD->Update(elapsedSec, m_pAvatar->GetNrLifes(), m_pAvatar->GetTotalPoints(), m_pGameObjectManager->GetTotalPrisoners());



}

void Game::Draw( ) const
{
	ClearBackground( );

	switch (m_GameState)
	{
	case Game::GameState::menu:
		m_pMenu->Draw();
		break;
	case Game::GameState::playing:
	case Game::GameState::bossFight:
	case Game::GameState::missionComplete:
		DrawPlaying();
		break;
	case Game::GameState::pause:
		DrawPlaying();
		m_pPause->Draw();  // Draw pause menu in front of everything
		break;
	case Game::GameState::gameOver:
		m_pGameOverScreen->Draw();
		break;

	}
}

// Draw the Playing GAME STATE
void  Game::DrawPlaying() const
{
	glPushMatrix();

		m_pCamera->Transform(m_pAvatar->GetBotShape(), m_pAvatar->GetGameStage());

		m_pLevel->DrawBackground();

		m_pGameObjectManager->Draw();

		m_pAvatar->Draw();

		m_pLevel->DrawForeground();

		m_pGameObjectManager->DrawBoss();

	glPopMatrix();

	// HUD will always stay at a fixed position
	m_pHUD->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{

	switch (m_GameState)
	{
	case Game::GameState::menu:
		// Enter key --> Start Playing
		if (e.keysym.sym == SDLK_RETURN)
		{
			m_pMenu->SetPlaying(true);
		}
		break;
	case Game::GameState::playing:
	case Game::GameState::bossFight:

		// X Key --> Shoot
		if (e.keysym.sym == SDLK_x)
		{
			m_pAvatar->Shoot();
		}

		// Pause game and show menu
		if (e.keysym.sym == SDLK_ESCAPE)
		{
			if (m_pPause == nullptr) // Check if pause menu was already loaded before
			{
				InitPause();
			}
			m_GameState = GameState::pause;
			m_pSoundManager->turnOffSound();  // Turn off the sound while in pause

		}
		break;
	case Game::GameState::pause:
		if (e.keysym.sym == SDLK_ESCAPE)
		{
			ClosePauseMenu();
		}
		break;
	}


	if (e.keysym.sym == SDLK_p)
	{
		// Turn on / off the sound 
		m_pSoundManager->turnOnOffSound();
	}
	
	if (e.keysym.sym == SDLK_y)
	{
		m_pSoundManager->ChangeSoundtrackVolume(10);
	}

	if (e.keysym.sym == SDLK_t)
	{
		m_pSoundManager->ChangeSoundtrackVolume(-10);
	}
	
	// I key --> Show controls game info at the console
	if (e.keysym.sym == SDLK_i)
	{
		DisplayControlsInfo();
	}



}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	
	if (m_GameState == GameState::pause)
	{
		m_MousePos.x = float(e.x);
		m_MousePos.y = float(e.y);
	}
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_GameState == GameState::pause)
	{
		Point2f mousePos{ float(e.x), float(e.y) };
		m_pPause->SelectOption(mousePos, m_CloseGame, m_ClosePauseMenu);

		if (m_ClosePauseMenu)
		{
			ClosePauseMenu();
		}
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

// Show in the console all the information about the game control keys
void Game::DisplayControlsInfo()
{
	std::cout << std::endl;
	std::cout << "-- METAL SLUG GAME CONTROLS INFO -- " << std::endl;
	std::cout << std::endl;
	std::cout << " - Use the left, Right and Up Arrow Keys to move the avatar throw the level." << std::endl;
	std::cout << " - Down Arrow key --> Avatar starts crawling. " << std::endl;
	std::cout << " - C key --> Jump." << std::endl;
	std::cout << " - X key --> Shoot." << std::endl;
	std::cout << " - ENTER key --> Start the Game (Only for the Start Screen Menu)" << std::endl;
	std::cout << " - ESC key --> Pause Game and show Pause Menu" << std::endl;
	std::cout << " - P key --> Mute / Unmute sound" << std::endl;
	std::cout << " - T key --> Decrement sound" << std::endl;
	std::cout << " - Y key --> Increment sound" << std::endl;

}

// Player wants to close the pause menu (either with the mouse or with the keyboard)
void Game::ClosePauseMenu()
{
	m_GameState = GameState::playing;
	m_ClosePauseMenu = false;
	m_pSoundManager->turnOnOffSound();  // Turn on the sounds again
}

void Game::ClearBackground( ) const
{
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

// Player wants to close the Game in the Pause Menu
bool Game::GetCloseGame() const
{
	return m_CloseGame;
}
