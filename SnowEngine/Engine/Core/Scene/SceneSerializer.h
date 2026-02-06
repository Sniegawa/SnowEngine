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

		void Serialize(std::filesystem::path filepath); // To scene file
		void SerializeRuntime(std::filesystem::path filepath); // To binary

		bool Deserialize(std::filesystem::path filepath);
		bool DeserializeRuntime(std::filesystem::path filepath);
	private:
		Ref<Scene> m_Scene;
	};
}