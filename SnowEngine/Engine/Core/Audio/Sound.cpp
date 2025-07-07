#include "Sound.h"
#include "AudioSystem.h"
namespace Snow
{
	ma_engine AudioSystem::s_Engine;

	Sound::Sound(const std::string& filepath)
	{
		auto Result = ma_sound_init_from_file(&AudioSystem::GetEngine(), filepath.c_str(), 0, nullptr, nullptr, &m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load sound from file");
		m_Loaded = true;
	}

	Sound::~Sound()
	{
		if (m_Loaded)
		{
			ma_sound_uninit(&m_Sound);
		}
	}
};