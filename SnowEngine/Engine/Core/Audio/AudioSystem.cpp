#include "AudioSystem.h"

namespace Snow
{
	ma_engine AudioSystem::s_Engine;
	SoundLibrary AudioSystem::s_SoundLibrary;

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
		SNOW_CORE_INFO("Audio system shutdown");
		ma_engine_uninit(&s_Engine);
	}

	void AudioSystem::Play(Ref<Sound>& sound)
	{
		sound->Play();
	}

	void AudioSystem::Play(const std::string& name)
	{
		s_SoundLibrary.Get(name)->Play();
	}

	void AudioSystem::Stop(Ref<Sound>& sound)
	{
		sound->Stop();
	}

	void AudioSystem::Stop(const std::string& name)
	{
		s_SoundLibrary.Get(name)->Stop();
	}

	void AudioSystem::SetSoundPosition(const Ref<Sound>& sound, const glm::vec2 position)
	{
		sound->SetPosition(position);
	}

	void AudioSystem::SetSoundPosition(const std::string& name, const glm::vec2 position)
	{
		s_SoundLibrary.Get(name)->SetPosition(position);
	}

	void AudioSystem::SetSoundPitch(const Ref<Sound>& sound, const float pitch)
	{
		sound->SetPitch(pitch);
	}

	void AudioSystem::SetSoundPitch(const std::string& name, const float pitch)
	{
		s_SoundLibrary.Get(name)->SetPitch(pitch);
	}

	void AudioSystem::SetSoundNearRadius(const Ref<Sound>& sound, const float nearRadius)
	{
		sound->SetNearRadius(nearRadius);
	}

	void AudioSystem::SetSoundNearRadius(const std::string& name, const float nearRadius)
	{
		s_SoundLibrary.Get(name)->SetNearRadius(nearRadius);
	}

	void AudioSystem::SetSoundFarRadius(const Ref<Sound>& sound, const float FarRadius)
	{
		sound->SetFarRadius(FarRadius);
	}

	void AudioSystem::SetSoundFarRadius(const std::string& name, const float FarRadius)
	{
		s_SoundLibrary.Get(name)->SetFarRadius(FarRadius);
	}

	void AudioSystem::SetMasterVolume(const float volume)
	{
		ma_engine_set_volume(&s_Engine, volume);
	}

	float AudioSystem::GetMasterVolume()
	{
		return ma_engine_get_volume(&s_Engine);
	}

	void AudioSystem::SetSoundAttenuationMode(const Ref<Sound>& sound, AttenuationModel model)
	{
		sound->SetAttenuationModel(model);
	}

	void AudioSystem::SetSoundAttenuationMode(const std::string& name, AttenuationModel model)
	{
		s_SoundLibrary.Get(name)->SetAttenuationModel(model);
	}


	void SoundLibrary::Add(const Ref<Sound>& sound, const std::string& name)
	{
		if (m_Sounds.find(name) != m_Sounds.end())
		{
			SNOW_CORE_ERROR("Sound with given name already exists : {0}", name);
			return;
		}
		m_Sounds[name] = sound;
	}

	Ref<Sound> SoundLibrary::Load(const std::string& name, const std::string& path)
	{
		auto it = m_Sounds.find(name);
		if (it != m_Sounds.end())
		{
			SNOW_CORE_WARN("Sound with given name already exists : {0}", name);
			return it->second;
		}
		auto sound = CreateRef<Sound>(name,path);
		Add(sound,name);
		return sound;
	}

	Ref<Sound> SoundLibrary::Get(const std::string& name)
	{
		auto it = m_Sounds.find(name);
		if (it == m_Sounds.end())
		{
			SNOW_CORE_ERROR("Sound with given name not found : {0}", name);
			return nullptr;
		}
		return it->second;

	}

	bool SoundLibrary::Unload(const std::string& name)
	{
		auto it = m_Sounds.find(name);
		if (it == m_Sounds.end())
		{
			SNOW_CORE_WARN("No sound with given name found for deletion {0}",name);
			return false;
		}
		m_Sounds.erase(it);
		SNOW_CORE_INFO("Sound {0} deleted succesfully", name);
		return true;
	}

	void SoundLibrary::Shutdown()
	{
		m_Sounds.clear();
	}
};