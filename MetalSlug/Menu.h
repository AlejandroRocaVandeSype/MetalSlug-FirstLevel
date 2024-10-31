#pragma once
#include "Game.h"

class Sprite;
class SoundManager;
class Menu
{
public:

	enum class State {

		startMenu,
		startAnimation,
		waitingToPlay

	};

	Menu(const Point2f& windowSize, SoundManager* soundManager);
	~Menu();
	Menu(const Menu& menu) = delete;
	Menu(Menu&& menu) = delete;
	Menu& operator=(const Menu& menu) = delete;
	Menu& operator=(Menu&& menu) = delete;


	void Draw() const;
	void Update(float elapsedSec, Game::GameState& gameState);

	void SetPlaying(bool start);	
	bool GetStartPlaying() const;

private:

	State m_MenuState;

	// Sound
	SoundManager* m_pSoundManager;
	SoundStream* m_pMenuSong;
	SoundEffect* m_pNarratorVoice;
	SoundEffect* m_pMenuSound;

	// Sprites for the menu
	Sprite* m_pBackground;
	Sprite* m_pPushStart;

	Sprite* m_pTitleTopPart;
	Sprite* m_pTitleBotPart;
	Sprite* m_pBulletCircle;
	Sprite* m_pBulletHole;

	const Point2f m_StartLettersPos;
	const Point2f m_WindowSize;
	const float m_TitleVel;
	float m_ScalingValue;
	const float m_StartAnimationValue;

	const float m_FlickeringMaxTime;
	float m_FlickeringSec;

	bool m_StartPlaying;
	bool m_DrawLetters;

	void Initialize();
	void UpdateFlicker(float elapsedSec);
	void UpdateMenuAnimation(float elapsedSec);
};

