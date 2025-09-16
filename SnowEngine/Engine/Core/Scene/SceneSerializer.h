#pragma once

#include "Scene.h"

//Note: I tried using reflection, but i can't figure it out to work properly,
// I don't really need to do reflection only it would make my life easier

namespace Snow
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath); // To scene file
		void SerializeRuntime(const std::string& filepath); // To binary

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}