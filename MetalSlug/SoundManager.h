#pragma once
#include <unordered_map>

class SoundStream;
class SoundEffect;
class SoundManager
{

public:

	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager& soundManager) = delete;
	SoundManager(SoundManager&& soundManager) = delete;
	SoundManager& operator=(const SoundManager& soundManager) = delete;
	SoundManager& operator=(SoundManager&& oundManager) = delete;

	SoundStream* GetSound(const std::string& filename);
	SoundEffect* GetEffect(const std::string& filename);
	int GetMusicVolume() const;
	int GetEffectsVolume() const;
	bool GetSoundActivated();

	void turnOnOffSound();	//Switch between turning on/off the sound
	void turnOffSound();	// Turn off the sound
	void PlaySoundEffect(const SoundEffect* soundEffect, int times);
	void PlaySong(const SoundStream* song, bool repeat);

	void ChangeSoundtrackVolume(int value);
	void ChangeEffectVolume(int value);

	
	

private:

	std::unordered_map < std::string, SoundStream* > m_Sounds;
	std::unordered_map < std::string, SoundEffect* > m_Effects;

	bool m_IsSoundOn;
	const int m_MaxVolume;			// Max volume for all sounds / effects
	int m_CurrentEffectsVolume;		// To know what is the current volume for all the effects
	int m_CurrentMusicVolume;
	

	
};

