#include "Sound.h"
#include "AudioSystem.h"
namespace Snow
{

	Sound::Sound(const std::string& name, const std::string& filepath)
		: m_Name(name)
	{
		auto Result = ma_sound_init_from_file(&AudioSystem::GetEngine(), filepath.c_str(), 0, nullptr, nullptr, &m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load sound from file");
		m_Loaded = true;
		ma_sound_set_volume(&m_Sound, m_Volume);
	}

	void Sound::SetVolume(float volume)
	{
		m_Volume = volume;
		ma_sound_set_volume(&m_Sound, m_Volume);
	}

	Sound::~Sound()
	{
		if (m_Loaded)
		{
			ma_sound_uninit(&m_Sound);
		}
	}
};