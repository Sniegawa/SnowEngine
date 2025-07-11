#include "Sound.h"
#include "AudioSystem.h"
namespace Snow
{

	Sound::Sound(const std::string& name, const std::string& filepath)
		: m_Name(name)
	{
		auto Result = ma_sound_init_from_file(&AudioSystem::GetEngine(), filepath.c_str(), MA_SOUND_FLAG_DECODE, nullptr, nullptr, &m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load sound from file");
		m_Loaded = true;
	}

	void Sound::Play()
	{
		auto Result = ma_sound_start(&m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
	}

	void Sound::Stop()
	{
		ma_sound_stop(&m_Sound);
	}

	float Sound::GetVolume()
	{
		return ma_sound_get_volume(&m_Sound);
	}

	void Sound::SetVolume(const float volume)
	{
		ma_sound_set_volume(&m_Sound, volume);
	}

	void Sound::SetPosition(const glm::vec2& position)
	{
		ma_sound_set_position(&m_Sound, position.x, position.y, 0.0f);
	}

	void Sound::SetPitch(const float pitch)
	{
		ma_sound_set_pitch(&m_Sound, pitch);
		
	}

	float Sound::GetPitch()
	{
		return ma_sound_get_pitch(&m_Sound);
	}

	void Sound::SetNearRadius(const float NearRadius)
	{
		ma_sound_set_min_distance(&m_Sound, NearRadius);
	}

	float Sound::GetNearRadius()
	{
		return ma_sound_get_min_distance(&m_Sound);
	}

	void Sound::SetFarRadius(const float FarRadius)
	{
		ma_sound_set_min_distance(&m_Sound, FarRadius);
	}

	float Sound::GetFarRadius()
	{
		return ma_sound_get_max_distance(&m_Sound);
	}

	void Sound::SetAttenuationModel(AttenuationModel model)
	{
		switch (model)
		{
		case Snow::AttenuationModel::Inverse:
			ma_sound_set_attenuation_model(&m_Sound, ma_attenuation_model_inverse);
			break;
		case Snow::AttenuationModel::Linear:
			ma_sound_set_attenuation_model(&m_Sound, ma_attenuation_model_linear);
			break;
		case Snow::AttenuationModel::Exponential:
			ma_sound_set_attenuation_model(&m_Sound, ma_attenuation_model_exponential);
			break;
		default:
			ma_sound_set_attenuation_model(&m_Sound, ma_attenuation_model_inverse);
			break;
		}
	}

	Sound::~Sound()
	{
		if (m_Loaded)
		{
			ma_sound_uninit(&m_Sound);
		}
	}
};