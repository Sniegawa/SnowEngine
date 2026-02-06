#include "ProjectManager.h"

#include <fstream>

#include <SnowEngineAPI.h>
#include <Core/Logging/Log.h>
#include <yaml-cpp/yaml.h>

#define KEY(x) << YAML::Key << x
#define VAL(x) << YAML::Value << x
#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap
#define MAP_END   << YAML::EndMap

namespace Snow
{
	ProjectManager::ProjectManager() : m_ProjectPath(), m_ProjectName("New Project"), m_AssetsPath(), m_ProjectFilePath() {}

	ProjectManager::~ProjectManager() {}

	void ProjectManager::SaveProject(std::filesystem::path& LastOpenedScene)
	{
		std::filesystem::path ProjectDirectoryPath = m_ProjectPath;		
		std::filesystem::path AssetsPath = m_AssetsPath;

		YAML::Emitter out;

		out MAP_START;
		out KEYVAL("SNPR", m_ProjectName);
		out KEYVAL("VERSION", 0.01);
		out KEYVAL("ASSETS_PATH", AssetsPath.string());
		out KEYVAL("LAST_SCENE", LastOpenedScene.string());
		out MAP_END;

		std::ofstream fout(m_ProjectFilePath);
		fout << out.c_str();
	}

	bool ProjectManager::OpenProject(std::filesystem::path path, std::filesystem::path& SceneToOpen)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			SNOW_CORE_ERROR("Failed to load .snpr file '{0}'\n     {1}", path.string(), e.what());
			return false;
		}
		if (!data["SNPR"])
			return false;

		m_ProjectName = data["SNPR"].as<std::string>();

		std::string AssetsPath = data["ASSETS_PATH"].as<std::string>();

		m_AssetsPath = AssetsPath;
		m_ProjectPath = path.parent_path();

		if (data["LAST_SCENE"])
			SceneToOpen = data["LAST_SCENE"].as<std::string>();

		m_ProjectFilePath = path;
		return true;
	}

	bool ProjectManager::CreateProject(std::filesystem::path path, std::string name)
	{
		std::filesystem::path ProjectDirectoryPath = path.parent_path();
		std::filesystem::path AssetsPath = ProjectDirectoryPath / "Assets";
		std::filesystem::create_directory(AssetsPath);
		YAML::Emitter out;

		out MAP_START;
		out KEYVAL("SNPR", name);
		out KEYVAL("VERSION", 0.01);
		out KEYVAL("ASSETS_PATH", AssetsPath.string());
		out KEYVAL("LAST_SCENE", "");
		out MAP_END;

		std::ofstream fout(path);
		fout << out.c_str();

		m_ProjectName = name;
		m_ProjectPath = ProjectDirectoryPath;
		m_AssetsPath = AssetsPath;
		m_ProjectFilePath = path;
		return true;
	}

}