#include "AudioSystem.h"

namespace Snow
{
	ma_engine AudioSystem::s_Engine;
	SoundLibrary AudioSystem::s_SoundLibrary;
	std::vector<Ref<SoundInstance>> AudioSystem::s_SoundInstances;

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
		s_SoundInstances.clear();
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
	}

	Ref<SoundInstance>& AudioSystem::Play(Ref<SoundAsset>& soundAsset)
	{
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(soundAsset);

		s_SoundInstances.push_back(instance);
		instance->Play();
		return instance;
	}

	Ref<SoundInstance>& AudioSystem::Play(const std::string& name)
	{
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(s_SoundLibrary.Get(name));

		s_SoundInstances.push_back(instance);
		instance->Play();
		return instance;
	}

	void AudioSystem::Stop(Ref<SoundInstance>& sound)
	{
		sound->Stop();
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

	void AudioSystem::SetSoundFarRadius(const Ref<SoundInstance>& sound, const float FarRadius)
	{
		sound->SetFarRadius(FarRadius);
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

	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path)
	{
		return s_SoundLibrary.Load(name, path);
	}

	Ref<SoundAsset> AudioSystem::GetSound(const std::string& name)
	{
		return s_SoundLibrary.Get(name);
	}


	void SoundLibrary::Add(const Ref<SoundAsset>& sound, const std::string& name)
	{
		if (m_Sounds.find(name) != m_Sounds.end())
		{
			SNOW_CORE_ERROR("SoundInstance with given name already exists : {0}", name);
			return;
		}
		m_Sounds[name] = sound;
	}

	Ref<SoundAsset> SoundLibrary::Load(const std::string& name, const std::string& path)
	{
		auto it = m_Sounds.find(name);
		if (it != m_Sounds.end())
		{
			SNOW_CORE_WARN("SoundInstance with given name already exists : {0}", name);
			return it->second;
		}
		auto sound = CreateRef<SoundAsset>(path);
		Add(sound, name);
		return sound;
	}

	Ref<SoundAsset> SoundLibrary::Get(const std::string& name)
	{
		auto it = m_Sounds.find(name);
		if (it == m_Sounds.end())
		{
			SNOW_CORE_ERROR("SoundInstance with given name not found : {0}", name);
			return nullptr;
		}
		return it->second;

	}

	bool SoundLibrary::Unload(const std::string& name)
	{
		auto it = m_Sounds.find(name);
		if (it == m_Sounds.end())
		{
			SNOW_CORE_WARN("No sound with given name found for deletion {0}", name);
			return false;
		}
		m_Sounds.erase(it);
		SNOW_CORE_INFO("SoundInstance {0} deleted succesfully", name);
		return true;
	}

	void SoundLibrary::Shutdown()
	{
		m_Sounds.clear();
	}
};