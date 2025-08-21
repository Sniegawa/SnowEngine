#pragma once

#include <miniaudio.h>
#include <glm/glm.hpp>

#include "AudioAssets.h"
#include <SnowEngineAPI.h>

namespace Snow
{

	class MusicInstance
	{
	public:
		MusicInstance(Ref<MusicAsset> asset);
		MusicInstance(Ref<MusicAsset> asset,MusicConfig& config);
		~MusicInstance();

		void Play();
		void Play(MusicConfig& config);
		void Stop();

		void SetLooping(bool loop);

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

		void ApplyConfig(MusicConfig& config);
		void ApplyConfig();
	public:
		MusicConfig m_Config;
	private:
		static void OnMusicEnd(void* pUserData, ma_sound* pMusic);


	private:
		ma_sound m_Music;

		Ref<MusicAsset> m_Asset;

		glm::vec2 m_Position;

		bool m_Loop;

		bool m_Finished = false;
	};
};