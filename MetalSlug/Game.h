#pragma once
#include "Level.h"
#include "Camera.h"
#include "SoundEffect.h"
#include "SoundStream.h"


class GameObject;
class Avatar;
class Enemy;
class Soldier;
class GameObjectManager;
class SoundManager;
class Boss;
class HUD;
class Menu;
class Pause;
class Game final
{
public:

	enum class GameState {

		menu,
		playing,
		bossFight,
		pause,						
		gameOver,					// Player has no more lifes left
		missionComplete				// Player finishes the level
	};

	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );


	bool GetCloseGame() const;

private:

	// DATA MEMBERS
	const Window m_Window;
	bool m_CloseGame;				// To know if player wants to close the game or not with the mouse
	bool m_ClosePauseMenu;

	GameState m_GameState;			// State of the Game

	// GAME Pointers
	Menu* m_pMenu;
	Level* m_pLevel;
	HUD* m_pHUD;
	Avatar* m_pAvatar;
	Camera* m_pCamera;
	Pause* m_pPause;

	Sprite* m_pGameOverScreen;

	// Sounds && Effects
	SoundManager* m_pSoundManager;
	SoundStream* m_pLevelSong;
	SoundStream* m_pBossSong;
	SoundStream* m_pGameOverSong;

	SoundEffect* m_pMissionStartSound;
	SoundEffect* m_pMissionComplete;

	// GameObjects
	GameObjectManager* m_pGameObjectManager;

	Point2f m_MousePos;

	// FUNCTIONS

	// Init Data 
	void InitMenu();
	void Initialize( );
	void InitAvatar();
	void InitCamera();
	void InitHUD();
	void InitLevelSounds();
	void InitGameOverState();
	void InitPause();

	// GameObjects
	void CreateGameObjectsFromFile();

	// STATES OF THE GAME
	void DrawPlaying() const;
	void UpdatePlaying(float elapsedSec);
	void UpdateFinishLevel(float elapsedSec);

	void DisplayControlsInfo();				// Display info about the controls of the game

	void Cleanup( );
	void ClearBackground( ) const;


	void ClosePauseMenu();

};