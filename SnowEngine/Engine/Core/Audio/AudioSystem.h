#pragma once

#include <miniaudio.h>
#include <SnowEngineAPI.h>

#include "AudioAssets.h"
#include "AudioInstance.h"

#include "../Timestep.h"

namespace Snow
{
	class AudioAssetLibrary
	{
	public:
		Ref<AudioAsset> Load(const std::string& name, const std::string& path, AudioType type = AudioType::SFX, const AudioConfig& config = AudioConfig());
		Ref<AudioAsset> Get(const std::string& name);

		bool Unload(const std::string& name);

		void Shutdown();

	private:
		void Add(const Ref<AudioAsset>& audioAsset, const std::string& name);
		std::unordered_map<std::string, Ref<AudioAsset>> m_Assets;
	};

	//Static Api class used to controlling audio in engine
	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update(Timestep ts);
		
		static Ref<AudioInstance> PlayAudio(const Ref<AudioAsset>& audioAsset, const AudioConfig& config = AudioConfig());
		static Ref<AudioInstance> PlayAudio(const std::string& name, const AudioConfig& config = AudioConfig());

		static void Stop(Ref<AudioInstance>& audio);

		static void SetAudioVolume(const Ref<AudioInstance>& audio, const float volume);
		static void SetAudioPosition(const Ref<AudioInstance>& audio, const glm::vec2& position);
		static void SetAudioPosition(const Ref<AudioInstance>& audio, const glm::vec3& position);
		static void SetAudioPitch(const Ref<AudioInstance>& audio, const float pitch);
		static void SetAudioNearRadius(const Ref<AudioInstance>& audio, const float nearRadius);
		static void SetAudioFarRadius(const Ref<AudioInstance>& audio, const float farRadius);
		static void SetAudioAttenuationModel(const Ref<AudioInstance>& audio, const AttenuationModel model);
		static void SetAudioLooping(const Ref<AudioInstance>& audio, bool loop);
		static void SetAudioConfig(const Ref<AudioInstance>& audio, const AudioConfig& config);

		static void SetMasterVolume(const float volume);
		static float GetMasterVolume();

		static Ref<AudioAsset> LoadAudio(const std::string& name, const std::string& path, AudioType type = AudioType::SFX, const AudioConfig& config = AudioConfig());
		static Ref<AudioAsset> GetAudio(const std::string& name);

		static ma_engine& GetEngine() { return s_Engine; }

		static void SetListenerPosition(const glm::vec3& position, uint32_t listenerID);

	private:
		static ma_engine s_Engine;
		static AudioAssetLibrary s_AudioLibrary;
		static std::vector<Ref<AudioInstance>> s_AudioInstances;
	};

};