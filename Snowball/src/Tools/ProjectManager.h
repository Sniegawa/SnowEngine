#pragma once

#include <SnowEngineAPI.h>
#include "Core/Asset/AssetType.h"

namespace Snow
{
	class ProjectManager
	{
	public:
		ProjectManager();
		~ProjectManager();

		bool OpenProject(Path path, Path& SceneToOpen);
		bool CreateProject(Path path, std::string name);

		void SaveProject(Path& LastOpenedScene);

		void LoadAssets(const Path& AssetsFolderPath);
		void MarkAssetDirty(const AssetEntry& entry);
		void CleanDirtyAssets();

		const inline Path& GetProjectPath() const { return m_ProjectPath; }

		const inline Path& GetAssetsPath() const { return m_AssetsPath; }

		const inline std::string& GetProjectName() const { return m_ProjectName; }
	private:
		std::string m_ProjectName;
		Path m_ProjectPath;
		Path m_AssetsPath;
		Path m_ProjectFilePath;
		// Project settings


		// Dirty assets
		std::vector<AssetEntry> m_DirtyAssets;
	};


};