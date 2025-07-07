#include "AudioSystem.h"

namespace Snow
{
	ma_engine AudioSystem::s_Engine;
	SoundLibrary AudioSystem::s_SoundLibrary;

	void AudioSystem::Init()
	{
		auto Result = ma_engine_init(nullptr, &s_Engine);
		SNOW_ASSERT(Result == MA_SUCCESS,"Couldn't initializy audio engine")
	}

	void AudioSystem::Shutdown()
	{
		s_SoundLibrary.Shutdown();
		SNOW_CORE_INFO("Audio system shutdown");
		ma_engine_uninit(&s_Engine);
	}

	void AudioSystem::Play(Ref<Sound>& sound)
	{
		auto Result = ma_sound_start(sound->GetSoundPointer());
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
	}

	void AudioSystem::Play(const std::string& name)
	{
		auto Result = ma_sound_start(s_SoundLibrary.Get(name)->GetSoundPointer());
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't play sound");
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

	void SoundLibrary::Shutdown()
	{
		m_Sounds.clear();
	}
};