#include "SceneSerializer.h"

#include <fstream>
#include <type_traits>

#include <yaml-cpp/yaml.h>

#include <glm/glm.hpp>
#include "Core/Logging/Log.h"
#include "Components.h"
#include "Entity.h"

#define KEY(x) << YAML::Key << x
#define VAL(x) << YAML::Value << x
#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap
#define MAP_END   << YAML::EndMap

namespace YAML {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

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

			out MAP_END;//SpriteRendererComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out KEY("CameraComponent");
			out MAP_START; //CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

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
