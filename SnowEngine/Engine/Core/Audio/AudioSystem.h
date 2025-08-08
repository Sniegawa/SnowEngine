#pragma once

#include <miniaudio.h>
#include <SnowEngineAPI.h>

#include "SoundInstance.h"
#include "SoundAsset.h"

namespace Snow
{
	class SoundLibrary
	{
	public:
		void Add(const Ref<SoundAsset>& sound, const std::string& name);
		Ref<SoundAsset> Load(const std::string& name, const std::string& path);
		Ref<SoundAsset> Get(const std::string& name);

		bool Unload(const std::string& name);

		void Shutdown();

	private:
		std::unordered_map<std::string, Ref<SoundAsset>> m_Sounds;
	};


	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update();

		static Ref<SoundInstance>& Play(Ref<SoundAsset>& soundAsset);
		static Ref<SoundInstance>& Play(const std::string& name);

		static void Stop(Ref<SoundInstance>& sound);

		static void SetSoundPosition(const Ref<SoundInstance>& sound, const glm::vec2 position);

		static void SetSoundPitch(const Ref<SoundInstance>& sound, const float pitch);

		static void SetSoundNearRadius(const Ref<SoundInstance>& sound, const float nearRadius);
		
		static void SetSoundFarRadius(const Ref<SoundInstance>& sound, const float FarRadius);

		static void SetMasterVolume(const float volume);
		static float GetMasterVolume();

		static void SetSoundAttenuationMode(const Ref<SoundInstance>& sound,AttenuationModel model);

		static Ref<SoundAsset> LoadSound(const std::string& name, const std::string& path);
		static Ref<SoundAsset> GetSound(const std::string& name);

		static ma_engine& GetEngine() { return s_Engine; }

		static size_t GetInstanceCount() { return s_SoundInstances.size(); }
	private:
		static ma_engine s_Engine;
		static SoundLibrary s_SoundLibrary;
		static std::vector<Ref<SoundInstance>> s_SoundInstances;
	};

};