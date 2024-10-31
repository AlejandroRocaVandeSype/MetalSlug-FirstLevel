#pragma once
#include <vector>
#include "SoundManager.h"

class SoundManager;
class Sprite;
class Pause
{
public:


	
	Pause(const Point2f& windowSize, SoundManager* soundManager);
	~Pause();
	Pause(const Pause& pause) = delete;
	Pause(Pause&& pause) = delete;
	Pause& operator=(const Pause& pause) = delete;
	Pause& operator=(Pause&& pause) = delete;


	void Draw() const;
	void Update(const Point2f& mousePos);

	void SelectOption(const Point2f& mousePos, bool& closeGame, bool& closePauseMenu);



private:
	
	enum class PauseState{

		mainPause,
		settings,
		controls

	};

	PauseState m_PauseState;
	Sprite* m_pMainPauseBackground;
	Sprite* m_pControlsBackground;
	Sprite* m_pSettingsBackground;
	Sprite* m_pMusicNr;
	Sprite* m_pEffectsNr;
	std::pair <Sprite*, Sprite*> m_pMusicArrows;	// It will contain the sprite with and without color	
	std::pair <Sprite*, Sprite*> m_pMusicArrowsR;
	std::pair <Sprite*, Sprite*> m_pEffectsArrows;
	std::pair <Sprite*, Sprite*> m_pEffectsArrowsR;
	std::pair <Sprite*, Sprite*> m_pBackTexts;
	std::vector< std::pair<Sprite*, Sprite*> > m_pMainPauseTexts;	
	std::vector< std::pair<Sprite*, Sprite*> > m_pVolumeArrows;

	SoundManager* m_pSoundManager;
	bool m_ClosePauseMenu;
	const int m_VolumeChangeValue;
	
	
	// INIT SPRITES
	void Initialize(const Point2f& windowSize);
	void InitBackgrounds(const Point2f& windowSize);
	void InitTexts(const Point2f& windowSize);
	void InitArrows();
	void InitVolumeValues();

	// DRAWINGS
	void DrawMainPause() const;
	void DrawSettings() const;
	void DrawControls() const;

	// UPDATES
	void UpdateMainPause(const Point2f& mousePos);
	void UpdateSettings(const Point2f& mousePos);
	void UpdateControls(const Point2f& mousePos);

	void HighlightSprite(const Point2f& mousePos, std::pair<Sprite*, Sprite*>& texts);
	void CalculateVolumeValue();

};

