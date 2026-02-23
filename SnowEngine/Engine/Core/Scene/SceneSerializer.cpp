#include "SceneSerializer.h"

#include <fstream>
#include <type_traits>

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

#include "Core/Logging/Log.h"
#include "Components.h"
#include "Entity.h"
#include "Utilities/YAML_GLM.h"

namespace Snow
{
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out MAP_START; // Entity
		if (entity.HasComponent<UUIDComponent>())
		{
			auto& id = entity.GetComponent<UUIDComponent>().id;
			
			out KEYVAL("Entity", UUIDToString(id));
		}

		if (entity.HasComponent<TagComponent>())
		{
			out KEY("TagComponent");
			out MAP_START; //TagComponent
			
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out KEYVAL("Tag",tag);

			out MAP_END; //TagComponent
			
		}
		if (entity.HasComponent<TransformComponent>())
		{
			out KEY("TransformComponent");
			out MAP_START; //TransformComponent
			
			auto& tc = entity.GetComponent<TransformComponent>();
			out KEYVAL("Translation", tc.Translation);
			out KEYVAL("Rotation", tc.Rotation);
			out KEYVAL("Scale", tc.Scale);

			out MAP_END;//TransformComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out KEY("SpriteRendererComponent");
			out MAP_START; //SpriteRendererComponent

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out KEYVAL("Color", src.Color);
			out KEYVAL("SpriteTextureID", UUIDToString(src.SpriteTexture));

			out MAP_END;//SpriteRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out KEY("CameraComponent");
			out MAP_START; //CameraComponent

			const auto& cameraComponent = entity.GetComponent<CameraComponent>();
			const auto& camera = cameraComponent.Camera;

			out KEY("Camera");
			out MAP_START; //Camera

			out KEYVAL("ProjectionType", (int)camera.GetProjectionType());
			out KEYVAL("PerspectiveFOV", camera.GetPerspectiveFOV());
			out KEYVAL("PerspectiveNear", camera.GetPerspectiveNearClip());
			out KEYVAL("PerspectiveFar", camera.GetPerspectiveFarClip());
			out KEYVAL("OrthographicSize", camera.GetOrthographicSize());
			out KEYVAL("OrthographicNear", camera.GetOrthographicNearClip());
			out KEYVAL("OrthographicFar", camera.GetOrthographicFarClip());
			out MAP_END; //Camera

			out KEYVAL("Primary", cameraComponent.Primary);
			out KEYVAL("FixedAspectRatio", cameraComponent.FixedAspectRatio);

			out MAP_END; //CameraComponent
		}

		if (entity.HasComponent<AudioEmitterComponent>())
		{
			out KEY("AudioEmitterComponent");
			out MAP_START;

			const auto& aec = entity.GetComponent<AudioEmitterComponent>();

			out KEYVAL("AudioID", UUIDToString(aec.Audio));
			out KEYVAL("Name", aec.Name);
			out MAP_END; // Audio Emitter Component
		}
		if(entity.HasComponent<AudioListenerComponent>())
		{
			out KEY("AudioListenerComponent");
			out MAP_START;

			const auto& alc = entity.GetComponent<AudioListenerComponent>();
			
			out KEYVAL("Volume", alc.masterVolume);

			out MAP_END;
		}
		out MAP_END; // Entity
	}

	void SceneSerializer::Serialize(std::filesystem::path filepath)
	{
		YAML::Emitter out;
		out MAP_START;
		out KEYVAL("Scene" ,m_Scene->m_SceneName);
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		auto view = m_Scene->m_Registry.view<TagComponent>();
		view.each([&](auto entityID, auto& tc)
		{
				Entity entity = Entity(entityID,m_Scene.get());
				if (!entity)
					return;

				SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out MAP_END;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(std::filesystem::path filepath)
	{
		//not implemented
		SNOW_CORE_ASSERT(false, "Serializing runtime not implemented");
	}

	bool SceneSerializer::Deserialize(std::filesystem::path filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			SNOW_CORE_ERROR("Failed to load .snow file '{0}'\n     {1}", filepath.string(), e.what());
			return false;
		}
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SNOW_CORE_INFO("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				auto idStr = entity["Entity"].as<std::string>();
				UUID id = UUIDFromString(idStr);
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SNOW_CORE_INFO("Deserialized entity with ID = {0}, name = {1}", idStr, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name,id);
				
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["SpriteTextureID"])
					{
						auto textureIDString = spriteRendererComponent["SpriteTextureID"].as<std::string>();
						src.SpriteTexture = UUIDFromString(textureIDString);
					}
				}
				auto audioEmitterComponent = entity["AudioEmitterComponent"];
				if(audioEmitterComponent)
				{
					auto& aec = deserializedEntity.AddComponent<AudioEmitterComponent>();
					if(audioEmitterComponent["AudioID"])
					{
						auto audioIDstring = audioEmitterComponent["AudioID"].as<std::string>();
						aec.Audio = UUIDFromString(audioIDstring);
						aec.Name = audioEmitterComponent["Name"].as<std::string>();
					}
				}
				auto audioListenerComponent = entity["AudioListenerComponent"];
				if(audioListenerComponent)
				{
					auto& alc = deserializedEntity.AddComponent<AudioListenerComponent>();
					alc.masterVolume = audioListenerComponent["Volume"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(std::filesystem::path filepath)
	{
		//not implemented
		SNOW_CORE_ASSERT(false,"Deserializing runtime not implemented");
		return false;
	}
};
