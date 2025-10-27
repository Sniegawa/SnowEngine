#include "AudioSystem.h"

namespace Snow
{
	//miniaudio sound engine
	ma_engine AudioSystem::s_Engine;
	//Libraries for containing audio assets
	AudioAssetLibrary AudioSystem::s_AudioLibrary;
	//Vectors of currently playing audio
	std::vector<Ref<AudioInstance>> AudioSystem::s_AudioInstances;

	/// <summary>
	/// Initializes the audio system, setting up the sound engine, default listener properties, and preallocating memory for sound and music instances.
	/// </summary>
	void AudioSystem::Init()
	{
		//Init sound engine
		auto Result = ma_engine_init(nullptr, &s_Engine);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't initialize audio engine");
		//This should be moved to scene, where it will be assigned on update
		ma_engine_listener_set_position(&s_Engine, 0, 0.0f, 0.0f, 0.0f);
		ma_engine_listener_set_direction(&s_Engine, 0, 0.0f, 0.0f, -1.0f);
		ma_engine_listener_set_world_up(&s_Engine, 0, 0, 1, 0);

		//Prealocate some memory to avoid resizing the vector
		const unsigned int DEFAULT_ALLOCATION_SIZE = 128;
		s_AudioInstances.reserve(DEFAULT_ALLOCATION_SIZE);
	}
	
	/// <summary>
	/// Shuts down the audio system, releasing all audio assets and resources.
	/// </summary>
	void AudioSystem::Shutdown()
	{
		//Clearup every audio asset before uninitializing engine
		s_AudioLibrary.Shutdown();
		s_AudioInstances.clear();
		SNOW_CORE_INFO("Audio system shutdown");
		ma_engine_uninit(&s_Engine);
	}

	/// <summary>
	/// Updates the audio system by removing finished sound and music instances.
	/// </summary>
	/// <param name="ts">The timestep representing the elapsed time since the last update.</param>
	void AudioSystem::Update(Timestep ts)
	{
		s_AudioInstances.erase(
			std::remove_if(
				s_AudioInstances.begin(), s_AudioInstances.end(),
				[](Ref<AudioInstance>& a) {return a->isFinished(); }),
			s_AudioInstances.end()
		);
	}

	Ref<AudioInstance> AudioSystem::PlayAudio(const Ref<AudioAsset>& audioAsset, const AudioConfig& config)
	{
		Ref<AudioInstance> instance = CreateRef<AudioInstance>(audioAsset,config);
		s_AudioInstances.push_back(instance);
		instance->Play();
		return instance;
	}

	Ref<AudioInstance> AudioSystem::PlayAudio(const std::string& name, const AudioConfig& config)
	{
		Ref<AudioInstance> instance = CreateRef<AudioInstance>(s_AudioLibrary.Get(name),config);
		s_AudioInstances.push_back(instance);
		instance->Play();
		return instance;
	}

	void AudioSystem::Stop(Ref<AudioInstance>& audio)
	{
		audio->Stop();
		s_AudioInstances.erase(
			std::remove(s_AudioInstances.begin(), s_AudioInstances.end(), audio),
			s_AudioInstances.end()
		);
	}

	/* 
	======================================
	|      API for calling instances     |
	======================================
	*/

	void AudioSystem::SetAudioVolume(const Ref<AudioInstance>& audio, const float volume)
	{
		audio->SetVolume(volume);
	}

	void AudioSystem::SetAudioPosition(const Ref<AudioInstance>& audio, const glm::vec2& position)
	{
		audio->SetPosition(position);
	}

	void AudioSystem::SetAudioPosition(const Ref<AudioInstance>& audio, const glm::vec3& position)
	{
		audio->SetPosition(position);
	}

	void AudioSystem::SetAudioPitch(const Ref<AudioInstance>& audio, const float pitch)
	{
		audio->SetPitch(pitch);
	}

	void AudioSystem::SetAudioNearRadius(const Ref<AudioInstance>& audio, const float nearRadius)
	{
		audio->SetNearRadius(nearRadius);
	}

	void AudioSystem::SetAudioFarRadius(const Ref<AudioInstance>& audio, const float farRadius)
	{
		audio->SetFarRadius(farRadius);
	}

	void AudioSystem::SetAudioAttenuationModel(const Ref<AudioInstance>& audio, const AttenuationModel model)
	{
		audio->SetAttenuationModel(model);
	}

	void AudioSystem::SetAudioLooping(const Ref<AudioInstance>& audio, bool loop)
	{
		audio->SetLooping(loop);
	}

	void AudioSystem::SetAudioConfig(const Ref<AudioInstance>& audio, const AudioConfig& config)
	{
		audio->ApplyConfig(config);
	}

	/// <summary>
	/// Sets the master volume for the audio system.
	/// </summary>
	/// <param name="volume">The desired master volume level as a floating-point value.</param>
	void AudioSystem::SetMasterVolume(const float volume)
	{
		ma_engine_set_volume(&s_Engine, volume);
	}

	/// <summary>
	/// Retrieves the current master volume level of the audio system.
	/// </summary>
	/// <returns>The master volume as a floating-point value.</returns>
	float AudioSystem::GetMasterVolume()
	{
		return ma_engine_get_volume(&s_Engine);
	}

	void AudioSystem::SetListenerPosition(const glm::vec3& position, uint32_t listenerID)
	{
		ma_engine_listener_set_position(&s_Engine, listenerID, position.x, position.y, position.z);
	}

	/*
	======================================
	|       API for audio library        |
	======================================
	*/

	Ref<AudioAsset> AudioSystem::LoadAudio(const std::string& name, const std::string& path, AudioType type, const AudioConfig& config)
	{
		return s_AudioLibrary.Load(name, path, type, config);
	}

	Ref<AudioAsset> AudioSystem::GetAudio(const std::string& name)
	{
		return s_AudioLibrary.Get(name);
	}


	void AudioAssetLibrary::Add(const Ref<AudioAsset>& audioAsset, const std::string& name)
	{
		//Check if asset exists
		if (m_Assets.find(name) != m_Assets.end())
		{

			//If it exists Error
			SNOW_CORE_ERROR("Audio asset with given name already exists : {0}", name);
			return;
		}
		//Assign asset by name
		m_Assets[name] = audioAsset;
	}

	Ref<AudioAsset> AudioAssetLibrary::Load(const std::string& name, const std::string& path, AudioType type, const AudioConfig& config)
	{
		//Search for asset to return it if it exists
		auto it = m_Assets.find(name);
		if (it != m_Assets.end())
		{
			SNOW_CORE_WARN("Audio asset with given name already exists : {0}", name);
			return it->second;
		}

		//If asset doesn't exist create Asset object with config
		auto audioAsset = CreateRef<AudioAsset>(path,type,config);
		//Add asset object
		Add(audioAsset, name);
		//Return asset handler
		return audioAsset;
	}

	Ref<AudioAsset> AudioAssetLibrary::Get(const std::string& name)
	{
		//Search for asset
		auto it = m_Assets.find(name);
		if (it == m_Assets.end())
		{
			//If it doesn't exist return nullptr and error
			SNOW_CORE_ERROR("Audio asset with given name not found : {0}", name);
			return nullptr;
		}
		//If it exists return it
		return it->second;
	}

	bool AudioAssetLibrary::Unload(const std::string& name)
	{
		//Search for asset
		auto it = m_Assets.find(name);
		if (it == m_Assets.end())
		{
			//If it doesn't exist return false and error
			SNOW_CORE_WARN("No audio asset with given name found for deletion {0}", name);
			return false;
		}
		//Erease asset
		m_Assets.erase(it);
		SNOW_CORE_INFO("Audio asset {0} deleted succesfully", name);
		return true;
	}

	void AudioAssetLibrary::Shutdown()
	{
		m_Assets.clear();
	}
};
