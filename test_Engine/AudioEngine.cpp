#include "AudioEngine.h"
#include "engineErrors.h"

namespace test_Engine
{
	void SoundEffect::play(int loops)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
	}

	void Music::play(int loops)
	{
		if (Mix_PlayMusic(m_music, loops))
			fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
	}

	void Music::pause()
	{
		Mix_PausedMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{
		// Empty
	}
	
	AudioEngine::~AudioEngine()
	{
		destroy();
	}
	
	void AudioEngine::init()
	{
		if (m_isInitialized)
			fatalError("Tried to init audio engine twice!");
		// Parameter can be a bitwise combination of MIX_INIT_FAC,
		// MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024))
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		m_isInitialized = true;
	}

	void AudioEngine::destroy()
	{
		if (m_isInitialized)
		{
			m_isInitialized = false;

			for (auto& it: m_effectMap)
				Mix_FreeChunk(it.second);

			for (auto& it: m_musicMap)
				Mix_FreeMusic(it.second);

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}
	
	SoundEffect AudioEngine::loadSoundEffect(const char* filePath)
	{
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		if (it == m_effectMap.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath);
			if (chunk == nullptr)
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			m_effectMap[filePath] = chunk;

			effect.m_chunk = chunk;
		}
		else
			effect.m_chunk = it->second;
		return effect;
	}
	Music AudioEngine::loadMusic(const char* filePath)
	{
		auto it = m_musicMap.find(filePath);

		Music effect;

		if (it == m_musicMap.end())
		{
			Mix_Music* music = Mix_LoadMUS(filePath);
			if (music == nullptr)
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			m_musicMap[filePath] = music;

			effect.m_music = music;
		}
		else
			effect.m_music = it->second;
		return effect;
	}



}