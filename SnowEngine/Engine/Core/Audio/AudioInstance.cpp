#include "AudioInstance.h"

#include "AudioSystem.h"

namespace Snow
{
	AudioInstance::AudioInstance(const Ref<AudioAsset> asset, const AudioConfig& config)
		: m_Asset(asset)
	{
		auto filePath = asset->filePath.string();
		//Create sound asset on miniaudio
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(),
			filePath.c_str(),
			config.type == AudioType::Music ? MA_SOUND_FLAG_STREAM : MA_SOUND_FLAG_DECODE,
			nullptr,
			nullptr,
			&m_Audio
		);

		//Apply given config
		ApplyConfig(config);

		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load audio from file");
	}

	AudioInstance::~AudioInstance()
	{
		ma_sound_uninit(&m_Audio);
	}

	void AudioInstance::OnMusicEnd(void* pUserData, ma_sound* pMusic)
	{
		auto* self = static_cast<AudioInstance*>(pUserData);
		//Set bool flag to finished so AudioSystem can delete it
		self->m_Finished = true;
	}

	void AudioInstance::Play()
	{
		//Play music
		auto Result = ma_sound_start(&m_Audio);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play audio");
		//Set end callback to function
		ma_sound_set_end_callback(&m_Audio, &AudioInstance::OnMusicEnd, this);
	}

	void AudioInstance::Play(const AudioConfig& config)
	{
		//Apply config
		ApplyConfig(config);
		//Play music
		auto Result = ma_sound_start(&m_Audio);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play audio");
		//Set end callback to function
		ma_sound_set_end_callback(&m_Audio, &AudioInstance::OnMusicEnd, this);
	}

	void AudioInstance::Stop()
	{
		//Stop music NOT PAUSE!!
		ma_sound_stop(&m_Audio);
	}

	void AudioInstance::ApplyConfig(const AudioConfig& config)
	{
		//Apply given config and set it as member variable
		SetVolume(config.volume);
		SetPitch(config.pitch);
		SetNearRadius(config.minDistance);
		SetFarRadius(config.maxDistance);
		SetAttenuationModel(config.attenuation);
		SetLooping(config.loop);
		m_Config = config;
	}

	void AudioInstance::ApplyConfig()
	{
		//Apply given config
		SetVolume(m_Config.volume);
		SetPitch(m_Config.pitch);
		SetNearRadius(m_Config.minDistance);
		SetFarRadius(m_Config.maxDistance);
		SetAttenuationModel(m_Config.attenuation);
		SetLooping(m_Config.loop);
	}

	void AudioInstance::SetLooping(bool loop)
	{
		//Set music looping
		//This will be viable only at the end of current playtrough of music
		//This won't stop or restart, just tell audio engine what to do on end
		ma_sound_set_looping(&m_Audio, loop);
	}

	float AudioInstance::GetVolume()
	{
		return ma_sound_get_volume(&m_Audio);
	}

	void AudioInstance::SetVolume(const float volume)
	{
		ma_sound_set_volume(&m_Audio, std::clamp(volume, 0.0f, 1.0f));
	}

	void AudioInstance::SetPosition(const glm::vec3& position)
	{
		ma_sound_set_position(&m_Audio, position.x, position.y, position.z);
		m_Position = position;
	}

	void AudioInstance::SetPosition(const glm::vec2& position)
	{
		SetPosition(glm::vec3(position.x, position.y, m_Position.z));
	}

	void AudioInstance::SetPitch(const float pitch)
	{
		ma_sound_set_pitch(&m_Audio, pitch);
	}

	float AudioInstance::GetPitch()
	{
		return ma_sound_get_pitch(&m_Audio);
	}

	void AudioInstance::SetNearRadius(const float NearRadius)
	{
		ma_sound_set_min_distance(&m_Audio, NearRadius);
	}

	float AudioInstance::GetNearRadius()
	{
		return ma_sound_get_min_distance(&m_Audio);
	}

	void AudioInstance::SetFarRadius(const float FarRadius)
	{
		ma_sound_set_min_distance(&m_Audio, FarRadius);
	}

	float AudioInstance::GetFarRadius()
	{
		return ma_sound_get_max_distance(&m_Audio);
	}

	void AudioInstance::SetAttenuationModel(AttenuationModel model)
	{
		switch (model)
		{
		case AttenuationModel::Inverse:
			ma_sound_set_attenuation_model(&m_Audio, ma_attenuation_model_inverse);
			break;
		case AttenuationModel::Linear:
			ma_sound_set_attenuation_model(&m_Audio, ma_attenuation_model_linear);
			break;
		case AttenuationModel::Exponential:
			ma_sound_set_attenuation_model(&m_Audio, ma_attenuation_model_exponential);
			break;
		default:
			ma_sound_set_attenuation_model(&m_Audio, ma_attenuation_model_inverse);
			break;
		}
	}
};