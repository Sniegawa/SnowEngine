#pragma once

#include <miniaudio.h>

#include "MusicAsset.h"
#include <SnowEngineAPI.h>


namespace Snow
{
	class MusicInstance
	{
	public:
		MusicInstance(Ref<MusicAsset> asset,bool loop = false);
		~MusicInstance();

		void Play();
		void Stop();

		void SetLooping(bool loop) { m_Loop = loop; }

		const bool& isFinished() const { return m_Finished; }

	private:
		static void OnMusicEnd(void* pUserData, ma_sound* pMusic);
	private:
		ma_sound m_Music;

		Ref<MusicAsset> m_Asset;

		bool m_Loop;

		bool m_Finished = false;
	};
};