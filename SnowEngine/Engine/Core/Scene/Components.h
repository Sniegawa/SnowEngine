#pragma once

#include <glm/glm.hpp>
#include "Core/Timestep.h"
#include "SnowEngineAPI.h"
#include "Core/Logging/Log.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Core/Audio/AudioAssets.h"
#include "Core/Audio/SoundInstance.h"
#include "Core/Audio/MusicInstance.h"

#include "Core/Audio/AudioSystem.h"

namespace Snow
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default; //Default copy constructor
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		operator std::string() { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; //Default copy constructor
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) { }

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default; //Default copy constructor
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () { return Color; }
	};

	struct CameraComponent
	{
		SceneCamera Camera = SceneCamera();
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		// Function pointers
		ScriptableEntity* (*InstantiateScript)(); 
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };

		}
	};

	struct AudioListenerComponent
	{
		uint32_t ListenerID = 0; // For now i support only 1 listener leave this for possible splitscreen
		float masterVolume = 1.0f;

		AudioListenerComponent() = default;
	};

	struct SoundEmitterComponent
	{
		Ref<SoundAsset> Sound = nullptr;
		SoundConfig Config = SoundConfig();
		
		void Play()
		{
			if (!Sound)
			{
				SNOW_CORE_WARN("Tried playing sound with no asset");
				return;
			}
			if (!isPlaying || Instance.expired())
			{
				Instance = CreateWeakRef<SoundInstance>(AudioSystem::SoundPlay(Sound, Config));
				isPlaying = true;
			}
		}

		void Stop()
		{
			auto instance = Instance.lock();
			AudioSystem::Stop(instance);
			isPlaying = false;
		}

		WeakRef<SoundInstance> Instance;
		bool isPlaying = false;

		SoundEmitterComponent() = default;
		SoundEmitterComponent(Ref<SoundAsset>& sound)
			: Sound(sound) {}
	};

	struct MusicEmitterComponent
	{
		Ref<MusicAsset> Music;
		MusicConfig config;

		MusicEmitterComponent(Ref<MusicAsset>& music, MusicConfig& cfg = MusicConfig())
			: Music(music), config(cfg) {}
	};
}