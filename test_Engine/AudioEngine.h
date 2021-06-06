#pragma once

#include <SDL_mixer.h>
#include <map>

namespace test_Engine
{
	class AudioEngine;

	class SoundEffect
	{
	public:
		friend AudioEngine;

		/// <summary>
		/// </summary>
		/// <param name="loops"> if loops == -1, loop forever, overwise play it loops+1 times</param>
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};

	class Music
	{
	public:
		friend AudioEngine;

		/// <summary>
		/// </summary>
		/// <param name="loops"> if loops == -1, loop forever, overwise play it loops times</param>
		void play(int loops = 0);

		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const char* filePath);
		Music loadMusic(const char* filePath);
	private:

		std::map<const char*, Mix_Chunk*> m_effectMap;
		std::map<const char*, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};
}

