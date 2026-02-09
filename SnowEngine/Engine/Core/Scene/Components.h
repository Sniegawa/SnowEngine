#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "SnowEngineAPI.h"
#include "Core/Timestep.h"
#include "Core/Logging/Log.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Core/Audio/AudioSystem.h"
#include "Core/Audio/AudioAssets.h"
#include "Core/Audio/AudioInstance.h"

#include "Core/Renderer/Texture.h"

#include "Utilities/UUID.h"
#include "Core/Asset/AssetType.h"

namespace Snow
{
	struct UUIDComponent
	{
		UUID id = UUID();
		UUIDComponent() = default;
		UUIDComponent(const UUID& _id) : id(_id) {}
	};

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
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };
		AssetID SpriteTexture;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default; //Default copy constructor
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color), SpriteTexture() { }

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

	struct AudioEmitterComponent
	{
		Ref<AudioAsset> Audio = nullptr;
		AudioConfig Config = AudioConfig();

		void Play()
		{
			if (!Audio)
			{
				SNOW_CORE_WARN("Tried playing audio with no asset");
				return;
			}
			if (!isPlaying || Instance.expired())
			{
				Instance = CreateWeakRef<AudioInstance>(AudioSystem::PlayAudio(Audio, Config));
				isPlaying = true;
			}
		}

		void Stop()
		{
			if(Instance.expired())
			{
				SNOW_CORE_WARN("Tried stopping audio that isn't playing");
				return;
			}
			auto instance = Instance.lock();
			AudioSystem::Stop(instance);
			isPlaying = false;
		}

		void ApplyConfig()
		{
			if(!Instance.expired())
			{
				auto instance = Instance.lock();
				instance->ApplyConfig(Config);
			}
		}
		constexpr bool IsPlaying() const { return isPlaying; }

		WeakRef<AudioInstance> Instance;
		bool isPlaying = false;

		AudioEmitterComponent()
			: Audio(nullptr), Config(AudioConfig()) {}
		AudioEmitterComponent(Ref<AudioAsset>& audio)
			: Audio(audio) {}
	};
}
