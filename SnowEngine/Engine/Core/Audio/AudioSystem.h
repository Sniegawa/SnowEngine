#pragma once

#include <miniaudio.h>
#include <SnowEngineAPI.h>

#include "SoundInstance.h"
#include "AudioAssets.h"

#include "MusicInstance.h"

#include "../Timestep.h"

namespace Snow
{
	template<typename T, typename C>
	class AudioAssetLibrary
	{
	public:
		Ref<T> Load(const std::string& name, const std::string& path);
		Ref<T> Load(const std::string& name, const std::string& path, C& config);
		Ref<T> Get(const std::string& name);

		bool Unload(const std::string& name);

		void Shutdown();

	private:
		void Add(const Ref<T>& audioAsset, const std::string& name);
		std::unordered_map<std::string, Ref<T>> m_Assets;
	};

	//Static Api class used to controlling audio in engine
	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();

		static void Update(Timestep ts);
		
		static Ref<SoundInstance>& SoundPlay(Ref<SoundAsset>& soundAsset);
		static Ref<SoundInstance>& SoundPlay(const std::string& name);
		static Ref<SoundInstance>& SoundPlay(Ref<SoundAsset>& soundAsset, SoundConfig& config);
		static Ref<SoundInstance>& SoundPlay(const std::string& name, SoundConfig& config);

		static Ref<MusicInstance>& MusicPlay(Ref<MusicAsset>& musicAsset);
		static Ref<MusicInstance>& MusicPlay(const std::string& name);
		static Ref<MusicInstance>& MusicPlay(Ref<MusicAsset>& musicAsset, MusicConfig& config);
		static Ref<MusicInstance>& MusicPlay(const std::string& name, MusicConfig& config);

		//Dunno how to handle stops
		static void Stop(Ref<SoundInstance>& sound);
		static void Stop(Ref<MusicInstance>& music);

		//Clamped [0-1]
		static void SetSoundVolume(const Ref<MusicInstance>& music, const float volume);
		static void SetSoundPosition(const Ref<SoundInstance>& sound, const glm::vec2 position);
		static void SetSoundPitch(const Ref<SoundInstance>& sound, const float pitch);
		static void SetSoundNearRadius(const Ref<SoundInstance>& sound, const float nearRadius);
		static void SetSoundFarRadius(const Ref<SoundInstance>& sound, const float farRadius);
		static void SetSoundAttenuationMode(const Ref<SoundInstance>& sound,AttenuationModel model);
		static void SetSoundConfig(const Ref<SoundInstance>& sound, SoundConfig& config);

		//Clamped [0-1]
		static void SetMusicVolume(const Ref<MusicInstance>& music, const float volume);
		static void SetMusicPosition(const Ref<MusicInstance>& music, const glm::vec2 position);
		static void SetMusicPitch(const Ref<MusicInstance>& music, const float pitch);
		static void SetMusicNearRadius(const Ref<MusicInstance>& music, const float nearRadius);
		static void SetMusicFarRadius(const Ref<MusicInstance>& music, const float farRadius);
		static void SetMusicAttenuationMode(const Ref<MusicInstance>& music, AttenuationModel model);
		static void SetMusicLooping(const Ref<MusicInstance>& music, bool loop);
		static void SetMusicConfig(const Ref<MusicInstance>& music, MusicConfig& config);

		static void SetMasterVolume(const float volume);
		static float GetMasterVolume();


		static Ref<SoundAsset> LoadSound(const std::string& name, const std::string& path);
		static Ref<SoundAsset> LoadSound(const std::string& name, const std::string& path, SoundConfig& config);
		static Ref<SoundAsset> GetSound(const std::string& name);

		static Ref<MusicAsset> LoadMusic(const std::string& name, const std::string& path);
		static Ref<MusicAsset> LoadMusic(const std::string& name, const std::string& path, MusicConfig& config);
		static Ref<MusicAsset> GetMusic(const std::string& name);

		static ma_engine& GetEngine() { return s_Engine; }

		static size_t GetInstanceCount() { return s_SoundInstances.size() + s_MusicInstances.size(); }

	private:
		static ma_engine s_Engine;
		static AudioAssetLibrary<SoundAsset,SoundConfig> s_SoundLibrary;
		static AudioAssetLibrary<MusicAsset,MusicConfig> s_MusicLibrary;
		static std::vector<Ref<SoundInstance>> s_SoundInstances;
		static std::vector<Ref<MusicInstance>> s_MusicInstances;
	};

};