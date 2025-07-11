#pragma once

#include <miniaudio.h>
#include <SnowEngineAPI.h>

#include "Sound.h"

namespace Snow
{
	class SoundLibrary
	{
	public:
		void Add(const Ref<Sound>& sound, const std::string& name);
		Ref<Sound> Load(const std::string& name, const std::string& path);
		Ref<Sound> Get(const std::string& name);

		bool Unload(const std::string& name);

		void Shutdown();

	private:
		std::unordered_map<std::string, Ref<Sound>> m_Sounds;
	};


	class AudioSystem
	{
	public:
		static void Init();
		static void Shutdown();

		//For now I'll leave play with sound class, but ultimately user should use Play() by string name
		static void Play(Ref<Sound>& sound); 
		static void Play(const std::string& name);

		static void Stop(Ref<Sound>& sound);
		static void Stop(const std::string& name);

		static void SetSoundPosition(const Ref<Sound>& sound, const glm::vec2 position);
		static void SetSoundPosition(const std::string& name, const glm::vec2 position);

		static void SetSoundPitch(const Ref<Sound>& sound, const float pitch);
		static void SetSoundPitch(const std::string& name, const float pitch);

		static void SetSoundNearRadius(const Ref<Sound>& sound, const float nearRadius);
		static void SetSoundNearRadius(const std::string& name, const float nearRadius);
		
		static void SetSoundFarRadius(const Ref<Sound>& sound, const float FarRadius);
		static void SetSoundFarRadius(const std::string& name, const float FarRadius);

		static void SetMasterVolume(const float volume);
		static float GetMasterVolume();

		static void SetSoundAttenuationMode(const Ref<Sound>& sound,AttenuationModel model);
		static void SetSoundAttenuationMode(const std::string& name,AttenuationModel model);

		static Ref<Sound> LoadSound(const std::string& name, const std::string& path) { return s_SoundLibrary.Load(name, path); }
		static Ref<Sound> GetSound(const std::string& name) { return s_SoundLibrary.Get(name); }

		static ma_engine& GetEngine() { return s_Engine; }

	private:
		static ma_engine s_Engine;
		static SoundLibrary s_SoundLibrary;
	};

};