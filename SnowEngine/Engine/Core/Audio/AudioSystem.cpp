#include "AudioSystem.h"

namespace Snow
{
	ma_engine AudioSystem::s_Engine;
	AudioAssetLibrary<SoundAsset> AudioSystem::s_SoundLibrary;
	AudioAssetLibrary<MusicAsset> AudioSystem::s_MusicLibrary;
	std::vector<Ref<SoundInstance>> AudioSystem::s_SoundInstances;
	std::vector<Ref<MusicInstance>> AudioSystem::s_MusicInstances;

	void AudioSystem::Init()
	{
		auto Result = ma_engine_init(nullptr, &s_Engine);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't initialize audio engine");

		ma_engine_listener_set_position(&s_Engine, 0, 0.0f, 0.0f, 0.0f); //For now use one listener and setting pos to 0.0f
		ma_engine_listener_set_direction(&s_Engine, 0, 0.0f, 0.0f, -1.0f);
		ma_engine_listener_set_world_up(&s_Engine, 0, 0, 1, 0);
	}

	void AudioSystem::Shutdown()
	{
		s_SoundLibrary.Shutdown();
		s_MusicLibrary.Shutdown();
		s_SoundInstances.clear();
		s_MusicInstances.clear();
		SNOW_CORE_INFO("Audio system shutdown");
		ma_engine_uninit(&s_Engine);
	}

	void AudioSystem::Update()
	{
		s_SoundInstances.erase(
			std::remove_if(
				s_SoundInstances.begin(), s_SoundInstances.end(),
				[](auto& s) {return s->isFinished(); }),
			s_SoundInstances.end()
		);

		s_MusicInstances.erase(
			std::remove_if(
				s_MusicInstances.begin(), s_MusicInstances.end(),
				[](auto& s) {return s->isFinished(); }),
			s_MusicInstances.end()
		);
	}

	Ref<SoundInstance>& AudioSystem::SoundPlay(Ref<SoundAsset>& soundAsset)
	{
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(soundAsset);
		s_SoundInstances.push_back(instance);

		instance->Play();
		return instance;
	}

	Ref<SoundInstance>& AudioSystem::SoundPlay(const std::string& name)
	{
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(s_SoundLibrary.Get(name));
		s_SoundInstances.push_back(instance);

		instance->Play();
		return instance;
	}

	Ref<MusicInstance>& AudioSystem::MusicPlay(Ref<MusicAsset>& musicAsset, bool Loop)
	{
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(musicAsset,Loop);
		s_MusicInstances.push_back(instance);

		instance->Play();
		return instance;
	}

	Ref<MusicInstance>& AudioSystem::MusicPlay(const std::string& name, bool Loop)
	{
		
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(s_MusicLibrary.Get(name),Loop);
		s_MusicInstances.push_back(instance);

		instance->Play();
		return instance;
	}

	void AudioSystem::Stop(Ref<SoundInstance>& sound)
	{
		sound->Stop();
		//For sfx i would like to destroy the sound instance maybe 
		//I'll reconsider it later
		s_SoundInstances.erase(
			std::remove(s_SoundInstances.begin(), s_SoundInstances.end(), sound),
			s_SoundInstances.end()
		);
	}

	void AudioSystem::Stop(Ref<MusicInstance>& music)
	{
		music->Stop();
	}

	void AudioSystem::SetSoundVolume(const Ref<MusicInstance>& sound, const float volume)
	{
		sound->SetVolume(volume);
	}

	void AudioSystem::SetSoundPosition(const Ref<SoundInstance>& sound, const glm::vec2 position)
	{
		sound->SetPosition(position);
	}

	void AudioSystem::SetSoundPitch(const Ref<SoundInstance>& sound, const float pitch)
	{
		sound->SetPitch(pitch);
	}

	void AudioSystem::SetSoundNearRadius(const Ref<SoundInstance>& sound, const float nearRadius)
	{
		sound->SetNearRadius(nearRadius);
	}

	void AudioSystem::SetSoundFarRadius(const Ref<SoundInstance>& sound, const float farRadius)
	{
		sound->SetFarRadius(farRadius);
	}

	void AudioSystem::SetMasterVolume(const float volume)
	{
		ma_engine_set_volume(&s_Engine, volume);
	}

	float AudioSystem::GetMasterVolume()
	{
		return ma_engine_get_volume(&s_Engine);
	}

	void AudioSystem::SetSoundAttenuationMode(const Ref<SoundInstance>& sound, AttenuationModel model)
	{
		sound->SetAttenuationModel(model);
	}

	void AudioSystem::SetSoundConfig(const Ref<SoundInstance>& sound, SoundConfig& config)
	{
		sound->ApplyConfig(config);
	}

	void AudioSystem::SetMusicVolume(const Ref<MusicInstance>& music, const float volume)
	{
		music->SetVolume(volume);
	}

	void AudioSystem::SetMusicPosition(const Ref<MusicInstance>& music, const glm::vec2 position)
	{
		music->SetPosition(position);
	}

	void AudioSystem::SetMusicPitch(const Ref<MusicInstance>& music, const float pitch)
	{
		music->SetPitch(pitch);
	}

	void AudioSystem::SetMusicNearRadius(const Ref<MusicInstance>& music, const float nearRadius)
	{
		music->SetNearRadius(nearRadius);
	}

	void AudioSystem::SetMusicFarRadius(const Ref<MusicInstance>& music, const float farRadius)
	{
		music->SetFarRadius(farRadius);
	}

	void AudioSystem::SetMusicAttenuationMode(const Ref<MusicInstance>& music, AttenuationModel model)
	{
		music->SetAttenuationModel(model);
	}

	void AudioSystem::SetMusicLooping(const Ref<MusicInstance>& music, bool loop)
	{
		music->SetLooping(loop);
	}

	void AudioSystem::SetMusicConfig(const Ref<MusicInstance>& music, MusicConfig& config)
	{
		music->ApplyConfig(config);
	}

	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path)
	{
		return s_SoundLibrary.Load(name, path);
	}

	Ref<SoundAsset> AudioSystem::GetSound(const std::string& name)
	{
		return s_SoundLibrary.Get(name);
	}

	Ref<MusicAsset> AudioSystem::LoadMusic(const std::string& name, const std::string& path)
	{
		return s_MusicLibrary.Load(name, path);
	}

	Ref<MusicAsset> AudioSystem::GetMusic(const std::string& name)
	{
		return s_MusicLibrary.Get(name);
	}

	template<typename T>
	void AudioAssetLibrary<T>::Add(const Ref<T>& sound, const std::string& name)
	{
		if (m_Assets.find(name) != m_Assets.end())
		{
			SNOW_CORE_ERROR("SoundInstance with given name already exists : {0}", name);
			return;
		}
		m_Assets[name] = sound;
	}
	template<typename T>
	Ref<T> AudioAssetLibrary<T>::Load(const std::string& name, const std::string& path)
	{
		auto it = m_Assets.find(name);
		if (it != m_Assets.end())
		{
			SNOW_CORE_WARN("SoundInstance with given name already exists : {0}", name);
			return it->second;
		}
		auto sound = CreateRef<T>(path);
		Add(sound, name);
		return sound;
	}
	template<typename T>
	Ref<T> AudioAssetLibrary<T>::Get(const std::string& name)
	{
		auto it = m_Assets.find(name);
		if (it == m_Assets.end())
		{
			SNOW_CORE_ERROR("SoundInstance with given name not found : {0}", name);
			return nullptr;
		}
		return it->second;

	}
	template<typename T>
	bool AudioAssetLibrary<T>::Unload(const std::string& name)
	{
		auto it = m_Assets.find(name);
		if (it == m_Assets.end())
		{
			SNOW_CORE_WARN("No sound with given name found for deletion {0}", name);
			return false;
		}
		m_Assets.erase(it);
		SNOW_CORE_INFO("SoundInstance {0} deleted succesfully", name);
		return true;
	}
	template<typename T>
	void AudioAssetLibrary<T>::Shutdown()
	{
		m_Assets.clear();
	}
};