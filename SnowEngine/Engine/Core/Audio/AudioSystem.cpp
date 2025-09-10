#include "AudioSystem.h"

namespace Snow
{
	//miniaudio sound engine
	ma_engine AudioSystem::s_Engine;
	//Libraries for containing audio assets
	AudioAssetLibrary<SoundAsset,SoundConfig> AudioSystem::s_SoundLibrary;
	AudioAssetLibrary<MusicAsset,MusicConfig> AudioSystem::s_MusicLibrary;
	//Vectors of currently playing audio (might revert from using pointers here)
	std::vector<Ref<SoundInstance>> AudioSystem::s_SoundInstances;
	std::vector<Ref<MusicInstance>> AudioSystem::s_MusicInstances;

	/// <summary>
	/// Initializes the audio system, setting up the sound engine, default listener properties, and preallocating memory for sound and music instances.
	/// </summary>
	void AudioSystem::Init()
	{
		//Init sound engine
		auto Result = ma_engine_init(nullptr, &s_Engine);
		SNOW_ASSERT(Result == MA_SUCCESS, "Couldn't initialize audio engine");

		//I'll modify this when i'm done with ecs and have audio listener component
		//That i can attach to player, only support one listener
		ma_engine_listener_set_position(&s_Engine, 0, 0.0f, 0.0f, 0.0f);
		ma_engine_listener_set_direction(&s_Engine, 0, 0.0f, 0.0f, -1.0f);
		ma_engine_listener_set_world_up(&s_Engine, 0, 0, 1, 0);

		//Prealocate some memory to avoid resizing the vector
		s_SoundInstances.reserve(128);
		s_MusicInstances.reserve(128);
	}
	
	/// <summary>
	/// Shuts down the audio system, releasing all audio assets and resources.
	/// </summary>
	void AudioSystem::Shutdown()
	{
		//Clearup every audio asset before uninitializing engine
		s_SoundLibrary.Shutdown();
		s_MusicLibrary.Shutdown();
		s_SoundInstances.clear();
		s_MusicInstances.clear();
		SNOW_CORE_INFO("Audio system shutdown");
		ma_engine_uninit(&s_Engine);
	}

	/// <summary>
	/// Updates the audio system by removing finished sound and music instances.
	/// </summary>
	/// <param name="ts">The timestep representing the elapsed time since the last update.</param>
	void AudioSystem::Update(Timestep ts)
	{
		//Clear sounds when they end
		s_SoundInstances.erase(
			std::remove_if(
				s_SoundInstances.begin(), s_SoundInstances.end(),
				[](auto& s) {return s->isFinished(); }),
			s_SoundInstances.end()
		);
		//Clear music when they end
		s_MusicInstances.erase(
			std::remove_if(
				s_MusicInstances.begin(), s_MusicInstances.end(),
				[](auto& s) {return s->isFinished(); }),
			s_MusicInstances.end()
		);
	}

	/// <summary>
	/// Creates and plays a sound instance from the given sound asset.
	/// </summary>
	/// <param name="soundAsset">A ref to the sound asset to be played.</param>
	/// <returns>A ref to the created and playing sound instance.</returns>
	Ref<SoundInstance> AudioSystem::SoundPlay(Ref<SoundAsset>& soundAsset)
	{
		//Create Sound instance by given asset
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(soundAsset);
		//Store it in currently playing sounds
		s_SoundInstances.push_back(instance);
		//Play sound
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Plays a sound by its name and returns a reference to the sound instance.
	/// </summary>
	/// <param name="name">The name of the sound to play.</param>
	/// <returns>A ref to the SoundInstance representing the currently playing sound.</returns>
	Ref<SoundInstance> AudioSystem::SoundPlay(const std::string& name)
	{
		//Create sound instance by given name
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(s_SoundLibrary.Get(name));
		//Store it in currently playing sounds
		s_SoundInstances.push_back(instance);
		//Play sound
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Plays a sound using the specified sound asset and configuration, returning a reference to the created sound instance.
	/// </summary>
	/// <param name="soundAsset">Ref to the sound asset to be played.</param>
	/// <param name="config">Configuration settings for the sound playback.</param>
	/// <returns>Ref to the created and playing SoundInstance.</returns>
	Ref<SoundInstance> AudioSystem::SoundPlay(Ref<SoundAsset>& soundAsset, SoundConfig& config)
	{
		//Create Sound instance by given asset and config
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(soundAsset, config);
		//Store it in currently playing sounds
		s_SoundInstances.push_back(instance);
		//Play sound
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Plays a sound by name with the specified configuration and returns a ref to the sound instance.
	/// </summary>
	/// <param name="name">The name of the sound to play.</param>
	/// <param name="config">The configuration settings for the sound playback.</param>
	/// <returns>A ref to the created and playing SoundInstance.</returns>
	Ref<SoundInstance> AudioSystem::SoundPlay(const std::string& name, SoundConfig& config)
	{
		//Create sound instance by given name and config
		Ref<SoundInstance> instance = CreateRef<SoundInstance>(s_SoundLibrary.Get(name), config);
		//Store it in currently playing sounds
		s_SoundInstances.push_back(instance);
		//Play sound
		instance->Play(config);
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Creates and plays a music instance from the given music asset, returning a ref to the instance.
	/// </summary>
	/// <param name="musicAsset">A ref to the music asset to be played.</param>
	/// <returns>A ref to the created and playing MusicInstance.</returns>
	Ref<MusicInstance> AudioSystem::MusicPlay(Ref<MusicAsset>& musicAsset)
	{
		//Create music instance by given asset
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(musicAsset);
		//Store it in currently playing music
		s_MusicInstances.push_back(instance);
		//Play music
		instance->Play();
		//Return instance handle
		return instance;
	}
	
	/// <summary>
	/// Plays a music track by name and returns a ref to the music instance.
	/// </summary>
	/// <param name="name">The name of the music track to play.</param>
	/// <returns>A ref to the MusicInstance representing the playing music.</returns>
	Ref<MusicInstance> AudioSystem::MusicPlay(const std::string& name)
	{
		//Create music instance by given name
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(s_MusicLibrary.Get(name));
		//Store it in currently playing music
		s_MusicInstances.push_back(instance);
		//Play music
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Starts playback of a music asset with the specified configuration and returns a ref to the music instance.
	/// </summary>
	/// <param name="musicAsset">Ref to the music asset to be played.</param>
	/// <param name="config">Configuration settings for music playback.</param>
	/// <returns>Ref to the created and playing music instance.</returns>
	Ref<MusicInstance> AudioSystem::MusicPlay(Ref<MusicAsset>& musicAsset,MusicConfig& config)
	{
		//Create music instance by given asset and config
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(musicAsset, config);
		//Store it in currently playing music
		s_MusicInstances.push_back(instance);
		//Play music
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Plays a music track by name with the specified configuration and returns a ref to the music instance.
	/// </summary>
	/// <param name="name">The name of the music track to play.</param>
	/// <param name="config">The configuration settings for the music instance.</param>
	/// <returns>A ref to the created and playing MusicInstance.</returns>
	Ref<MusicInstance> AudioSystem::MusicPlay(const std::string& name,MusicConfig& config)
	{
		//Create music instance by given name and config
		Ref<MusicInstance> instance = CreateRef<MusicInstance>(s_MusicLibrary.Get(name), config);
		//Store it in currently playing music
		s_MusicInstances.push_back(instance);
		//Play music
		instance->Play();
		//Return instance handle
		return instance;
	}

	/// <summary>
	/// Stops playback of the specified sound instance and removes it from the list of active sound instances.
	/// </summary>
	/// <param name="sound">A ref to the sound instance to be stopped and removed.</param>
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

	/// <summary>
	/// Stops playback of the specified music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance to stop.</param>
	void AudioSystem::Stop(Ref<MusicInstance>& music)
	{
		music->Stop();
    //Since i would need to implement pausing myself and pausing != stoping ill delete it as in above
    s_MusicInstances.erase(
      std::remove(s_MusicInstances.begin(), s_MusicInstances.end(),music),
      s_MusicInstances.end()
    );
	}

	/* 
	======================================
	|      API for calling instances     |
	======================================
	*/

	/// <summary>
	/// Sets the volume for a specified sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose volume will be set.</param>
	/// <param name="volume">The desired volume level as a floating-point value. Clamped [0-1]</param>
	void AudioSystem::SetSoundVolume(const Ref<SoundInstance>& sound, const float volume)
	{
		sound->SetVolume(volume);
	}

	/// <summary>
	/// Sets the position of a sound instance in 2D space.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose position will be set.</param>
	/// <param name="position">A 2D vector specifying the new position</param>
	void AudioSystem::SetSoundPosition(const Ref<SoundInstance>& sound, const glm::vec2 position)
	{
		sound->SetPosition(position);
	}

	/// <summary>
	/// Sets the pitch of a given sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose pitch will be set.</param>
	/// <param name="pitch">The new pitch value to apply to the sound instance.</param>
	void AudioSystem::SetSoundPitch(const Ref<SoundInstance>& sound, const float pitch)
	{
		sound->SetPitch(pitch);
	}

	/// <summary>
	/// Sets the near radius for a sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose near radius will be set.</param>
	/// <param name="nearRadius">The new near radius value to assign to the sound instance.</param>
	void AudioSystem::SetSoundNearRadius(const Ref<SoundInstance>& sound, const float nearRadius)
	{
		sound->SetNearRadius(nearRadius);
	}

	/// <summary>
	/// Sets the far radius for a given sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose far radius will be set.</param>
	/// <param name="farRadius">The new far radius value to assign to the sound instance.</param>
	void AudioSystem::SetSoundFarRadius(const Ref<SoundInstance>& sound, const float farRadius)
	{
		sound->SetFarRadius(farRadius);
	}

	/// <summary>
	/// Sets the attenuation model for a given sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance whose attenuation model will be set.</param>
	/// <param name="model">The attenuation model to apply to the sound instance.</param>
	void AudioSystem::SetSoundAttenuationModel(const Ref<SoundInstance>& sound, AttenuationModel model)
	{
		sound->SetAttenuationModel(model);
	}

	/// <summary>
	/// Applies a sound configuration to a sound instance.
	/// </summary>
	/// <param name="sound">A ref to the sound instance to configure.</param>
	/// <param name="config">The sound configuration to apply.</param>
	void AudioSystem::SetSoundConfig(const Ref<SoundInstance>& sound, SoundConfig& config)
	{
		sound->ApplyConfig(config);
	}

	/// <summary>
	/// Sets the volume for a specified music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance whose volume will be set.</param>
	/// <param name="volume">The desired volume level as a floating-point value. Clamped [0-1]</param>
	void AudioSystem::SetMusicVolume(const Ref<MusicInstance>& music, const float volume)
	{
		music->SetVolume(volume);
	}

	/// <summary>
	/// Sets the position of a music instance in 2D space.
	/// </summary>
	/// <param name="music">A ref to the music instance whose position will be set.</param>
	/// <param name="position">A 2D vector specifying the new position</param>
	void AudioSystem::SetMusicPosition(const Ref<MusicInstance>& music, const glm::vec2 position)
	{
		music->SetPosition(position);
	}

	/// <summary>
	/// Sets the pitch of a music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance whose pitch will be set.</param>
	/// <param name="pitch">The new pitch value to apply to the music instance.</param>
	void AudioSystem::SetMusicPitch(const Ref<MusicInstance>& music, const float pitch)
	{
		music->SetPitch(pitch);
	}

	/// <summary>
	/// Sets the near radius for a music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance whose near radius will be set.</param>
	/// <param name="nearRadius">The new near radius value to assign to the music instance.</param>
	void AudioSystem::SetMusicNearRadius(const Ref<MusicInstance>& music, const float nearRadius)
	{
		music->SetNearRadius(nearRadius);
	}

	/// <summary>
	/// Sets the far radius for a music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance whose far radius will be set.</param>
	/// <param name="farRadius">The new far radius value to assign to the music instance.</param>
	void AudioSystem::SetMusicFarRadius(const Ref<MusicInstance>& music, const float farRadius)
	{
		music->SetFarRadius(farRadius);
	}

	/// <summary>
	/// Sets the attenuation model for a music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance whose attenuation model will be set.</param>
	/// <param name="model">The attenuation model to apply to the music instance.</param>
	void AudioSystem::SetMusicAttenuationModel(const Ref<MusicInstance>& music, AttenuationModel model)
	{
		music->SetAttenuationModel(model);
	}

	/// <summary>
	/// Sets whether the specified music instance should loop during playback.
	/// </summary>
	/// <param name="music">A ref to the music instance whose looping behavior will be set.</param>
	/// <param name="loop">True to enable looping; false to disable looping.</param>
	void AudioSystem::SetMusicLooping(const Ref<MusicInstance>& music, bool loop)
	{
		music->SetLooping(loop);
	}

	/// <summary>
	/// Applies a configuration to a music instance.
	/// </summary>
	/// <param name="music">A ref to the music instance to configure.</param>
	/// <param name="config">The music configuration to apply.</param>
	void AudioSystem::SetMusicConfig(const Ref<MusicInstance>& music, MusicConfig& config)
	{
		music->ApplyConfig(config);
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

	/// <summary>
	/// Loads a sound asset from the specified file path and associates it with a given name.
	/// </summary>
	/// <param name="name">The name to associate with the loaded sound asset.</param>
	/// <param name="path">The file path to the sound asset to be loaded.</param>
	/// <returns>A ref to the loaded SoundAsset.</returns>
	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path)
	{
		return s_SoundLibrary.Load(name, path);
	}

	/// <summary>
	/// Loads a sound asset from the specified path with the given configuration.
	/// </summary>
	/// <param name="name">The name to assign to the loaded sound asset.</param>
	/// <param name="path">The file system path to the sound file to load.</param>
	/// <param name="config">The configuration settings to use when loading the sound.</param>
	/// <returns>A ref to the loaded SoundAsset.</returns>
	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path, SoundConfig& config)
	{
		return s_SoundLibrary.Load(name, path, config);
	}

	/// <summary>
	/// Retrieves a sound asset by its name from the sound library.
	/// </summary>
	/// <param name="name">The name of the sound asset to retrieve.</param>
	/// <returns>A ref to the requested SoundAsset.</returns>
	Ref<SoundAsset> AudioSystem::GetSound(const std::string& name)
	{
		return s_SoundLibrary.Get(name);
	}

	/// <summary>
	/// Loads a music asset from the specified path and adds it to the music library.
	/// </summary>
	/// <param name="name">The name to assign to the music asset.</param>
	/// <param name="path">The file system path to the music asset to load.</param>
	/// <returns>A ref to the loaded MusicAsset.</returns>
	Ref<MusicAsset> AudioSystem::LoadMusic(const std::string& name, const std::string& path)
	{
		return s_MusicLibrary.Load(name, path);
	}

	/// <summary>
	/// Loads a music asset into the audio system using the specified name, file path, and configuration.
	/// </summary>
	/// <param name="name">The name to assign to the music asset.</param>
	/// <param name="path">The file system path to the music asset to load.</param>
	/// <param name="config">The configuration settings for loading the music asset.</param>
	/// <returns>A ref to the loaded MusicAsset.</returns>
	Ref<MusicAsset> AudioSystem::LoadMusic(const std::string& name, const std::string& path,MusicConfig& config)
	{
		return s_MusicLibrary.Load(name, path,config);
	}

	/// <summary>
	/// Retrieves a music asset by its name from the music library.
	/// </summary>
	/// <param name="name">The name of the music asset to retrieve.</param>
	/// <returns>A ref to the requested musicAsset.</returns>
	Ref<MusicAsset> AudioSystem::GetMusic(const std::string& name)
	{
		return s_MusicLibrary.Get(name);
	}

	/// <summary>
	/// Adds an audio asset to the library with the specified name.
	/// If an asset with the given name already exists, logs an error and does not add the asset.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	/// <param name="audioAsset">A ref to the audio asset to add.</param>
	/// <param name="name">The name to associate with the audio asset.</param>
	template<typename T, typename C>
	void AudioAssetLibrary<T,C>::Add(const Ref<T>& audioAsset, const std::string& name)
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

	/// <summary>
	/// Loads an audio asset by name and path, using the provided configuration.
	/// If an asset with the given name already exists, logs an error and does not add the asset.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	/// <param name="name">The unique name identifying the audio asset.</param>
	/// <param name="path">The file system path to the audio asset.</param>
	/// <returns>Ref to the loaded audio asset.</returns>
	template<typename T, typename C>
	Ref<T> AudioAssetLibrary<T,C>::Load(const std::string& name, const std::string& path)
	{
		//Search for asset to return it if it exists
		auto it = m_Assets.find(name);
		if (it != m_Assets.end())
		{
			SNOW_CORE_WARN("Audio asset with given name already exists : {0}", name);
			return it->second;
		}

		//If asset doesn't exist create Asset object
		auto audioAsset = CreateRef<T>(path);
		//Add asset object
		Add(audioAsset, name);
		//Return asset handler
		return audioAsset;
	}

	/// <summary>
	/// Loads an audio asset by name and path, using the provided configuration. If the asset already exists, returns the existing asset; otherwise, creates, adds, and returns a new asset.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	/// <param name="name">The unique name identifying the audio asset.</param>
	/// <param name="path">The file system path to the audio asset.</param>
	/// <param name="config">The configuration object used to initialize the audio asset.</param>
	/// <returns>A ref to the loaded audio asset.</returns>
	template<typename T, typename C>
	Ref<T> AudioAssetLibrary<T, C>::Load(const std::string& name, const std::string& path, C& config)
	{
		//Search for asset to return it if it exists
		auto it = m_Assets.find(name);
		if (it != m_Assets.end())
		{
			SNOW_CORE_WARN("Audio asset with given name already exists : {0}", name);
			return it->second;
		}

		//If asset doesn't exist create Asset object with config
		auto audioAsset = CreateRef<T>(path,config);
		//Add asset object
		Add(audioAsset, name);
		//Return asset handler
		return audioAsset;
	}

	/// <summary>
	/// Retrieves an audio asset ref by name from the asset library.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	/// <param name="name">The name of the audio asset to retrieve.</param>
	/// <returns>A ref to the audio asset if found; otherwise, nullptr.</returns>
	template<typename T, typename C>
	Ref<T> AudioAssetLibrary<T,C>::Get(const std::string& name)
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

	/// <summary>
	/// Removes an audio asset from the library by its name.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	/// <param name="name">The name of the audio asset to unload.</param>
	/// <returns>True if the asset was found and removed; false if no asset with the given name exists.</returns>
	template<typename T, typename C>
	bool AudioAssetLibrary<T,C>::Unload(const std::string& name)
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

	/// <summary>
	/// Clears all audio assets from the library.
	/// </summary>
	/// <typeparam name="T">The type of audio asset.</typeparam>
	/// <typeparam name="C">The config type for audio asset.</typeparam>
	template<typename T, typename C>
	void AudioAssetLibrary<T,C>::Shutdown()
	{
		m_Assets.clear();
	}
};
