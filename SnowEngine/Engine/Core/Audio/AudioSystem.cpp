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

	/*
	*	@brief Plays sound
	*	@param 1) Reference to sound asset
	*	@return	Reference to sound instance
	*/
	Ref<SoundInstance>& AudioSystem::SoundPlay(Ref<SoundAsset>& soundAsset)
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

	/*
	*	@brief Plays sound
	*	@param 1) Name of sound asset from library
	*	@return	Reference to sound instance
	*/
	Ref<SoundInstance>& AudioSystem::SoundPlay(const std::string& name)
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

	/*
	*	@brief Plays sound
	*	@param 1) Reference to sound asset
	*	@param 2) Sound config
	*	@return	Reference to sound instance
	*/
	Ref<SoundInstance>& AudioSystem::SoundPlay(Ref<SoundAsset>& soundAsset, SoundConfig& config)
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

	/*
	*	@brief Plays sound
	*	@param 1) Name of sound asset from library 
	*	@param 2) Sound config
	*	@return	Reference to sound instance
	*/
	Ref<SoundInstance>& AudioSystem::SoundPlay(const std::string& name, SoundConfig& config)
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

	/*
	*	@brief Plays music
	*	@param 1) Reference to music asset
	*	@return	Reference to music instance
	*/
	Ref<MusicInstance>& AudioSystem::MusicPlay(Ref<MusicAsset>& musicAsset)
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
	/*
	*	@brief Plays music
	*	@param 1) Name of music asset from library
	*	@return	Reference to music instance
	*/
	Ref<MusicInstance>& AudioSystem::MusicPlay(const std::string& name)
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

	/*
	*	@brief Plays music
	*	@param 1) Reference to music asset
	*	@param 2) Music config
	*	@return	Reference to music instance
	*/
	Ref<MusicInstance>& AudioSystem::MusicPlay(Ref<MusicAsset>& musicAsset,MusicConfig& config)
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

	/*
	*	@brief Plays music
	*	@param 1) Name of music asset from library
	*	@param 2) Music config
	*	@return	Reference to music instance
	*/
	Ref<MusicInstance>& AudioSystem::MusicPlay(const std::string& name,MusicConfig& config)
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

	/*
	*	@brief Stops given sound instance and destroys it
	*	@param 1) Reference to sound instance
	*/
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

	/*
	*	@brief Stops given music instance
	*	@param 1) Reference to music instance
	*/
	void AudioSystem::Stop(Ref<MusicInstance>& music)
	{
		music->Stop();
	}

	/* 
	======================================
	|      API for calling instances     |
	======================================
	*/

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

	void AudioSystem::SetMasterVolume(const float volume)
	{
		ma_engine_set_volume(&s_Engine, volume);
	}

	float AudioSystem::GetMasterVolume()
	{
		return ma_engine_get_volume(&s_Engine);
	}


	/*
	======================================
	|       API for loading audio        |
	======================================
	*/

	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path)
	{
		return s_SoundLibrary.Load(name, path);
	}

	Ref<SoundAsset> AudioSystem::LoadSound(const std::string& name, const std::string& path, SoundConfig& config)
	{
		return s_SoundLibrary.Load(name, path, config);
	}

	Ref<SoundAsset> AudioSystem::GetSound(const std::string& name)
	{
		return s_SoundLibrary.Get(name);
	}

	Ref<MusicAsset> AudioSystem::LoadMusic(const std::string& name, const std::string& path)
	{
		return s_MusicLibrary.Load(name, path);
	}

	Ref<MusicAsset> AudioSystem::LoadMusic(const std::string& name, const std::string& path,MusicConfig& config)
	{
		return s_MusicLibrary.Load(name, path,config);
	}

	Ref<MusicAsset> AudioSystem::GetMusic(const std::string& name)
	{
		return s_MusicLibrary.Get(name);
	}

	/*
	*	@brief Adds audio asset directly into library and assigns a name
	*   @tparam T - Type of asset
	*	@tparam C - Type of config
	*	@param 1) Reference to asset of type T
	*	@param 2) Name of the asset
	*/
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

	/*
	*	@brief Loads an audio asset with given config to library
	*   @tparam T - Type of asset
	*	@tparam C - Type of config
	*	@param 1) Name of the asset
	*	@param 2) Path to audio file
	*   @param 3) Audio asset configuration options
	*	@return	Reference to wanted audio asset
	*/
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

	/*
	*	@brief Loads an audio asset with given config to library
	*   @tparam T - Type of asset
	*	@tparam C - Type of config
	*	@param 1) Name of the asset
	*	@param 2) Path to audio file
	*   @param 3) Audio asset configuration options
	*	@return	Reference to wanted audio asset
	*/
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

	/*
	*	@brief Retrieves a reference to a audio asset from library
	* 	@tparam T - Type of asset
	*	@tparam C - Type of config
	*	@param 1) Name of the asset
	*	@return	Reference to wanted audio asset 
	*/
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

	/*
	*	@brief Unloads audio asset from library
	*   @tparam T - Type of asset
	*	@tparam C - Type of config
	*	@param 1) Name of the asset
	*	@return Bool based if function succeded or not
	*/
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
		m_Assets.erase(it);
		SNOW_CORE_INFO("Audio asset {0} deleted succesfully", name);
		return true;
	}

	template<typename T, typename C>
	void AudioAssetLibrary<T,C>::Shutdown()
	{
		m_Assets.clear();
	}
};