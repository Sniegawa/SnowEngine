#pragma once

#include <filesystem>

namespace Snow
{
	class ProjectManager
	{
	public:
		ProjectManager();
		~ProjectManager();

		bool OpenProject(std::filesystem::path path, std::filesystem::path& SceneToOpen);
		bool CreateProject(std::filesystem::path path, std::string name);

		void SaveProject(std::filesystem::path& LastOpenedScene);

		const inline std::filesystem::path& GetProjectPath() const { return m_ProjectPath; }

		const inline std::filesystem::path& GetAssetsPath() const { return m_AssetsPath; }

		const inline std::string& GetProjectName() const { return m_ProjectName; }
	private:
		std::string m_ProjectName;
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_AssetsPath;
		std::filesystem::path m_ProjectFilePath;
		// Project settings
	};


};