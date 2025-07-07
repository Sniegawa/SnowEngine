#include "AudioSystem.h"

namespace Snow
{
	void AudioSystem::Init()
	{
		auto Result = ma_engine_init(nullptr, &s_Engine);
		SNOW_ASSERT(Result == MA_SUCCESS,"Couldn't initializy audio engine")
	}

	void AudioSystem::Shutdown()
	{
		ma_engine_uninit(&s_Engine);
	}

	void AudioSystem::Play(Ref<Sound>& sound)
	{
		auto Result = ma_sound_start(sound->GetSoundPointer());
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
	}

};