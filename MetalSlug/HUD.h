#pragma once
#include <vector>

class Sprite;
class SoundManager;
class SoundEffect;
class Texture;
class HUD
{
public:


	HUD(const Point2f& bottomLeft, const Point2f& windowSize, SoundManager* sounds, int totalPrisoners);
	~HUD();
	HUD(const HUD& hud) = delete;
	HUD(HUD&& hud) = delete;
	HUD& operator=(const HUD& hud) = delete;
	HUD& operator=(HUD&& hud) = delete;


	void Draw() const;
	void Update(float elapsedSec, const int nrLifes, unsigned int totalPoints, unsigned int totalPrisoners);

	void ActivateGoTextAnimation();

	bool GetTimeOver() const;

	void SetEndLevel();

private:

	const Point2f m_BottomLeft;    // Position of the bottom Left of the window
	const Point2f m_WindowSize;

	const float m_TopBorder;
	const float m_LeftBorder;
	const float m_BottomBorder;

	

	int m_TotalPrisoners;		
	float m_Xpos;

	// Ending HUD
	const float m_LettersVel;
	float m_Seconds;
	bool m_IsEndLevel;
	bool m_IsBlackScreen;
	float m_AlphaValue;

	// GO Text animation
	bool m_GoAnimation;
	float m_MaxTimeGoAnimat;
	float m_TimeGoAnimat;
	SoundEffect* m_pGoSound;

	// SPRITES FOR THE HUD
	Sprite* m_pWeapons;
	Sprite* m_pPlayer;
	Sprite* m_pLevel;
	Sprite* m_pGo;
	Sprite* m_pNrLifes;
	Sprite* m_pMissionComplete;
	std::vector<Sprite*> m_pPrisoners;
	std::vector<Sprite*> m_pSystemPoint;   // Points system
	std::vector<Sprite*> m_pTimer;

	// Points System
	int m_UnitValue;
	int m_TenValue;
	int m_HundredValue;
	int m_ThousandValue;

	// Timer
	const int m_StartTime;
	int m_TimeLeft;
	float m_TimerSeconds;
	bool m_TimeOver;

	// Sounds
	SoundManager* m_pSoundManager;

	// PRIVATE FUNCTIONS
	void Initialize();
	void InitSystemPoint();
	void DrawPrisoners() const;
	void DrawSystemPoint() const;
	void DrawEndLevel() const;

	void UpdateGoText(float elapsedSec);
	void UpdateSystemPoints(float elapsedSec, unsigned int totalPoints);
	void UpdatePrisoners(unsigned int totalPrisoners);
	void UpdateTimer(float elapsedSec);
	void UpdateEndLevelHUD(float elapsedSec);

};

