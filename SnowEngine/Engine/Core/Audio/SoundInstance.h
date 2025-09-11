#pragma once

#include <miniaudio.h>
#include <string>
#include <glm/glm.hpp>
#include "AudioAssets.h"
#include <SnowEngineAPI.h>

namespace Snow
{

	class SoundInstance
	{
	public:
		SoundInstance(Ref<SoundAsset> asset);
		SoundInstance(Ref<SoundAsset> asset,SoundConfig& config);

		ma_sound* GetSoundPointer() { return &m_Sound; }

		void Play();
		void Play(SoundConfig& config);
		void Stop();

		float GetVolume();
		void SetVolume(const float volume);

		void SetPosition(const glm::vec3& position);
		void SetPosition(const glm::vec2& position);
		const glm::vec3& GetPosition() const { return m_Position; }

		void SetPitch(const float pitch);
		float GetPitch();

		void SetNearRadius(const float NearRadius);
		float GetNearRadius();
		void SetFarRadius(const float FarRadius);
		float GetFarRadius();

		void SetAttenuationModel(AttenuationModel model);

		const bool& isFinished() const { return m_Finished; }

		void ApplyConfig(SoundConfig& config);
		void ApplyConfig();

		~SoundInstance();
	public:
		SoundConfig m_Config;
	private:
		static void OnSoundEnd(void* pUserData, ma_sound* pSound);
	private:
		//Handle to miniaudio sound
		ma_sound m_Sound;

		//Handle to Sound asset
		Ref<SoundAsset> m_Asset;

		glm::vec3 m_Position;

		bool m_Finished = false;
	};
};