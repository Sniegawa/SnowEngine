#include "SoundInstance.h"
#include "AudioSystem.h"
#include "MusicInstance.h"

namespace Snow
{

	SoundInstance::SoundInstance(Ref<SoundAsset> asset)
		: m_Asset(asset)
	{
		//Create sound on audio engine
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(), 
			m_Asset->filePath.c_str(), 
			MA_SOUND_FLAG_DECODE, 
			nullptr, 
			nullptr, 
			&m_Sound
		);
		//Apply assets config
		ApplyConfig(asset->defaultConfig);

		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load sound from file");
	}

	SoundInstance::SoundInstance(Ref<SoundAsset> asset, SoundConfig& config)
		: m_Asset(asset)
	{
		//Create sound on audio engine
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(),
			m_Asset->filePath.c_str(),
			MA_SOUND_FLAG_DECODE,
			nullptr,
			nullptr,
			&m_Sound
		);
		//Apply given config
		ApplyConfig(config);

		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load sound from file");

	}

	void SoundInstance::Play()
	{
		//Play sound
		auto Result = ma_sound_start(&m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
		//Retrieve callback from miniaudio to function
		ma_sound_set_end_callback(&m_Sound, SoundInstance::OnSoundEnd, this);

	}

	void SoundInstance::Play(SoundConfig& config)
	{
		//Apply given config
		ApplyConfig(config);
		//Play sound
		auto Result = ma_sound_start(&m_Sound);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
		//Retrieve callback from miniaudio to function
		ma_sound_set_end_callback(&m_Sound, SoundInstance::OnSoundEnd, this);
	}

	void SoundInstance::Stop()
	{
		//Stop sound NOT PAUSE!!
		ma_sound_stop(&m_Sound);
	}

	void SoundInstance::ApplyConfig(SoundConfig& config)
	{
		//Apply given config and set is as member variable
		SetVolume(config.volume);
		SetPitch(config.pitch);
		SetNearRadius(config.minDistance);
		SetFarRadius(config.maxDistance);
		SetAttenuationModel(config.attenuation);
		m_Config = config;
	}

	void SoundInstance::ApplyConfig()
	{
		//Apply given config
		SetVolume(m_Config.volume);
		SetPitch(m_Config.pitch);
		SetNearRadius(m_Config.minDistance);
		SetFarRadius(m_Config.maxDistance);
		SetAttenuationModel(m_Config.attenuation);
	}

	float SoundInstance::GetVolume()
	{
		return ma_sound_get_volume(&m_Sound);
	}

	void SoundInstance::SetVolume(const float volume)
	{
		
		ma_sound_set_volume(&m_Sound, std::clamp(volume,0.0f,1.0f));
	}

	void SoundInstance::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		ma_sound_set_position(&m_Sound, position.x, position.y, position.z);
	}

	//Set Sound Origin for spatialized audio
	void SoundInstance::SetPosition(const glm::vec2& position)
	{
		SetPosition(glm::vec3(position.x,position.y,m_Position.z));
	}

	void SoundInstance::SetPitch(const float pitch)
	{
		ma_sound_set_pitch(&m_Sound, pitch);
	}

	float SoundInstance::GetPitch()
	{
		return ma_sound_get_pitch(&m_Sound);
	}

	void SoundInstance::SetNearRadius(const float NearRadius)
	{
		ma_sound_set_min_distance(&m_Sound, NearRadius);
	}

	float SoundInstance::GetNearRadius()
	{
		return ma_sound_get_min_distance(&m_Sound);
	}

	void SoundInstance::SetFarRadius(const float FarRadius)
	{
		ma_sound_set_min_distance(&m_Sound, FarRadius);
	}

	float SoundInstance::GetFarRadius()
	{
		return ma_sound_get_max_distance(&m_Sound);
	}

	void SoundInstance::SetAttenuationModel(AttenuationModel model)
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

	SoundInstance::~SoundInstance()
	{
		ma_sound_uninit(&m_Sound);
	}

	void SoundInstance::OnSoundEnd(void* pUserData, ma_sound* pSound)
	{
		auto* instance = static_cast<SoundInstance*>(pUserData);
		//Set bool flag to finished so AudioSystem can delete it
		instance->m_Finished = true; 
	}
};