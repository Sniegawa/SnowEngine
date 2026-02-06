#pragma once

#include <SnowEngineAPI.h>
#include <Core/Renderer/Texture.h>

#include <filesystem>


namespace Snow
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		

		void SetAssetsPath(std::filesystem::path path) { m_AssetsPath = path; m_CurrentDirectory = m_AssetsPath; }

		void OnImGuiRender();
	private:
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_SceneIcon;

		std::filesystem::path m_CurrentDirectory;
		std::filesystem::path m_AssetsPath;
	};
}