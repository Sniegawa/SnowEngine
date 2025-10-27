#pragma once

#include <miniaudio.h>
#include <glm/glm.hpp>

#include "AudioAssets.h"
#include <SnowEngineAPI.h>
namespace Snow
{

	class AudioInstance
	{
	public:
		AudioInstance(const Ref<AudioAsset> asset, const AudioConfig& config);
		~AudioInstance();

		void Play();
		void Play(const AudioConfig& config);

		void Stop();

		void SetLooping(bool loop);

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

		void ApplyConfig(const AudioConfig& config);
		void ApplyConfig();
	public:
		AudioConfig m_Config;
	private:
		static void OnMusicEnd(void* pUserData, ma_sound* pMusic);


	private:
		//miniaudio sound handle
		ma_sound m_Audio;

		//MusicAsset Handle
		Ref<AudioAsset> m_Asset;

		glm::vec3 m_Position;

		bool m_Loop;

		bool m_Finished = false;
	};
}