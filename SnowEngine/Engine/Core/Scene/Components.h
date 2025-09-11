#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; //Default copy constructor
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) { }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0,0,1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
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
		float masterVolume = 0.5f;

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

    constexpr bool IsPlaying() const { return isPlaying; }

		WeakRef<SoundInstance> Instance;
		bool isPlaying = false;

		SoundEmitterComponent() = default;
		SoundEmitterComponent(Ref<SoundAsset>& sound)
			: Sound(sound) {}
	};

	struct MusicEmitterComponent
	{
		Ref<MusicAsset> Music = nullptr;
		MusicConfig Config = MusicConfig();

    void Play()
    {
      if(!Music)
      {
        SNOW_CORE_WARN("Tried playing sound with no asset");
        return;
      }
      if(!isPlaying || Instance.expired())
      {
        Instance = CreateWeakRef<MusicInstance>(AudioSystem::MusicPlay(Music,Config));
        isPlaying = true;
      }
    }

    void Stop()
    {
      if(!Instance.expired())
      {
        isPlaying = false;
        return;
      }
      auto instance = Instance.lock();
      AudioSystem::Stop(instance);
      isPlaying = false;
    }

    bool IsPlaying() const { return isPlaying; }

    WeakRef<MusicInstance> Instance;
    bool isPlaying = false;

    MusicEmitterComponent() = default;
		MusicEmitterComponent(Ref<MusicAsset>& music)
			: Music(music) {}
	};
}
