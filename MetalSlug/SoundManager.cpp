#include "pch.h"
#include "SoundManager.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include <iostream>



SoundManager::SoundManager()
	: m_Sounds{}
	, m_Effects{}
	, m_IsSoundOn { true }
	, m_MaxVolume { 90 }
	, m_CurrentEffectsVolume { m_MaxVolume }
 	, m_CurrentMusicVolume { m_MaxVolume }
{

}

SoundManager::~SoundManager()
{
	for (std::pair<std::string, SoundStream*> sound : m_Sounds)
	{
		delete sound.second;
		
	}
	
	for (std::pair<std::string, SoundEffect*> effect : m_Effects)
	{
		delete effect.second;

	}

}


SoundStream* SoundManager::GetSound(const std::string& filename)
{

	if (m_Sounds.find(filename) == m_Sounds.end())
	{
		// It didn't find the sound
		SoundStream* pSound{ new SoundStream(filename) };   // Create it

		m_Sounds[filename] = pSound;			   // Add it to the container

		return pSound;
	}
	else
	{
		return m_Sounds[filename];
	}
}

SoundEffect* SoundManager::GetEffect(const std::string& filename)
{
	if (m_Effects.find(filename) == m_Effects.end())
	{
		SoundEffect* pEffect{ new SoundEffect(filename) };   

		pEffect->SetVolume(m_CurrentEffectsVolume);    // Put the volume of the effect with the current volume value

		m_Effects[filename] = pEffect;			  

		return pEffect;
	}
	else
	{
		return m_Effects[filename];
	}

}

int SoundManager::GetMusicVolume() const
{
	return m_CurrentMusicVolume;
}

int SoundManager::GetEffectsVolume() const
{
	return m_CurrentEffectsVolume;
}

// Play the sound effect as many times as indicated and only if the sound is on
void SoundManager::PlaySoundEffect(const SoundEffect* soundEffect, int times)
{
	if (m_IsSoundOn)
	{
		soundEffect->Play(times);
	}
}

void SoundManager::PlaySong(const SoundStream* song, bool repeat)
{
	if (m_IsSoundOn)
	{
		song->Play(repeat);
	}
}


// Decrement / Increment soundtrack volume according with the value
void SoundManager::ChangeSoundtrackVolume(int value)
{
	// Dont increase volume more than the max volume value
	if ( (value < 0 && m_CurrentMusicVolume > 0 ) || (value > 0 && m_CurrentMusicVolume < m_MaxVolume) )
	{
		m_CurrentMusicVolume += value;
		for (std::pair<std::string, SoundStream*> sound : m_Sounds)
		{
			sound.second->SetVolume(m_CurrentMusicVolume);
			// Changing the volume for one will change it for all the songs
			break;
		}
		
	}
	
}

void SoundManager::ChangeEffectVolume(int value)
{
	// Dont increase more than maxVolume
	if ( (value < 0 && m_CurrentEffectsVolume > 0) || (value > 0 && m_CurrentEffectsVolume < m_MaxVolume) )
	{
		m_CurrentEffectsVolume += value;
		for (std::pair<std::string, SoundEffect*> effect : m_Effects)
		{
			effect.second->SetVolume(m_CurrentEffectsVolume);
		}
	}
	
}

// Switch between On/Off 
void SoundManager::turnOnOffSound()
{
	
	if (m_IsSoundOn)
	{
		turnOffSound();
	}
	else
	{
		// turn on sound
		for (std::pair<std::string, SoundStream*> sound : m_Sounds)
		{
			sound.second->Resume();
		}

		for (std::pair<std::string, SoundEffect*> effect : m_Effects)
		{
			effect.second->ResumeAll();
		}

		m_IsSoundOn = true;
	}
	
}

void SoundManager::turnOffSound()
{
	if (m_IsSoundOn)   // If sound is activated
	{
		// Turn off sound

		for (std::pair<std::string, SoundStream*> sound : m_Sounds)
		{
			sound.second->Pause();
		}

		for (std::pair<std::string, SoundEffect*> effect : m_Effects)
		{
			effect.second->StopAll();
		}

		m_IsSoundOn = false;
	}
}

bool SoundManager::GetSoundActivated()
{
	return m_IsSoundOn;
}