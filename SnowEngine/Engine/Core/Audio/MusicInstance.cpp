#include "MusicInstance.h"
#include "AudioSystem.h"

namespace Snow
{

	MusicInstance::MusicInstance(Ref<MusicAsset> asset)
		: m_Asset(asset)
	{
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(),
			asset->filePath.c_str(),
			MA_SOUND_FLAG_STREAM,
			nullptr,
			nullptr,
			&m_Music
		);

		ApplyConfig(asset->defaultConfig);

		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load music from file");
	}

	MusicInstance::MusicInstance(Ref<MusicAsset> asset, MusicConfig& config)
		: m_Asset(asset)
	{
		auto Result = ma_sound_init_from_file(
			&AudioSystem::GetEngine(),
			asset->filePath.c_str(),
			MA_SOUND_FLAG_STREAM,
			nullptr,
			nullptr,
			&m_Music
		);

		ApplyConfig(config);

		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't load music from file");
	}

	MusicInstance::~MusicInstance()
	{
		ma_sound_uninit(&m_Music);
	}

	void MusicInstance::OnMusicEnd(void* pUserData, ma_sound* pMusic)
	{
		auto* self = static_cast<MusicInstance*>(pUserData);
		if (self->m_Loop)
		{
			//Restart streamed audio
			ma_sound_seek_to_pcm_frame(pMusic, 0);
			ma_sound_start(pMusic);
		}
		else
		{
			self->m_Finished = true;
		}
	}

	void MusicInstance::Play()
	{
		auto Result = ma_sound_start(&m_Music);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play music");
		ma_sound_set_pitch(&m_Music, 2.0f);
		ma_sound_set_end_callback(&m_Music, &MusicInstance::OnMusicEnd, this);
	}

	void MusicInstance::Play(MusicConfig& config)
	{
		auto Result = ma_sound_start(&m_Music);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play music");
		ma_sound_set_pitch(&m_Music, 2.0f);
		ma_sound_set_end_callback(&m_Music, &MusicInstance::OnMusicEnd, this);

		ApplyConfig(config);
	}

	void MusicInstance::Stop()
	{
		ma_sound_stop(&m_Music);
	}

	void MusicInstance::ApplyConfig(MusicConfig& config)
	{
		SetVolume(config.volume);
		SetPitch(config.pitch);
		SetNearRadius(config.minDistance);
		SetFarRadius(config.maxDistance);
		SetAttenuationModel(config.attenuation);
		m_Loop = config.looping;
		m_Config = config;
	}

	void MusicInstance::ApplyConfig()
	{
		SetVolume(m_Config.volume);
		SetPitch(m_Config.pitch);
		SetNearRadius(m_Config.minDistance);
		SetFarRadius(m_Config.maxDistance);
		SetAttenuationModel(m_Config.attenuation);
		m_Loop = m_Config.looping;
	}

	float MusicInstance::GetVolume()
	{
		return ma_sound_get_volume(&m_Music);
	}

	void MusicInstance::SetVolume(const float volume)
	{
		ma_sound_set_volume(&m_Music, volume);
	}

	void MusicInstance::SetPosition(const glm::vec2& position)
	{
		ma_sound_set_position(&m_Music, position.x, position.y, 0.0f);
	}

	void MusicInstance::SetPitch(const float pitch)
	{
		ma_sound_set_pitch(&m_Music, pitch);
	}

	float MusicInstance::GetPitch()
	{
		return ma_sound_get_pitch(&m_Music);
	}

	void MusicInstance::SetNearRadius(const float NearRadius)
	{
		ma_sound_set_min_distance(&m_Music, NearRadius);
	}

	float MusicInstance::GetNearRadius()
	{
		return ma_sound_get_min_distance(&m_Music);
	}

	void MusicInstance::SetFarRadius(const float FarRadius)
	{
		ma_sound_set_min_distance(&m_Music, FarRadius);
	}

	float MusicInstance::GetFarRadius()
	{
		return ma_sound_get_max_distance(&m_Music);
	}

	void MusicInstance::SetAttenuationModel(AttenuationModel model)
	{
		switch (model)
		{
		case AttenuationModel::Inverse:
			ma_sound_set_attenuation_model(&m_Music, ma_attenuation_model_inverse);
			break;
		case AttenuationModel::Linear:
			ma_sound_set_attenuation_model(&m_Music, ma_attenuation_model_linear);
			break;
		case AttenuationModel::Exponential:
			ma_sound_set_attenuation_model(&m_Music, ma_attenuation_model_exponential);
			break;
		default:
			ma_sound_set_attenuation_model(&m_Music, ma_attenuation_model_inverse);
			break;
		}
	}
};