#pragma once

#include <miniaudio.h>
#include <string>
#include <glm/glm.hpp>
#include "SoundAsset.h"
#include <SnowEngineAPI.h>

namespace Snow
{

	enum class AttenuationModel
	{
		Inverse,
		Linear,
		Exponential
	};

	class SoundInstance
	{
	public:
		SoundInstance(Ref<SoundAsset> asset);
		ma_sound* GetSoundPointer() { return &m_Sound; }

		void Play();
		void Stop();

		float GetVolume();
		void SetVolume(const float volume);

		void SetPosition(const glm::vec2& position);
		const glm::vec2& GetPosition() const { return m_Position; }

		void SetPitch(const float pitch);
		float GetPitch();

		void SetNearRadius(const float NearRadius);
		float GetNearRadius();
		void SetFarRadius(const float FarRadius);
		float GetFarRadius();

		void SetAttenuationModel(AttenuationModel model);

		const bool& isFinished() const { return m_Finished; }

		~SoundInstance();
	private:
		static void OnSoundEnd(void* pUserData, ma_sound* pSound);
	private:
		ma_sound m_Sound;

		Ref<SoundAsset> m_Asset;

		glm::vec2 m_Position;

		bool m_Finished = false;
	};
};