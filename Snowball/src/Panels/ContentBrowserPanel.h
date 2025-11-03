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

		void OnImGuiRender();
	private:
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_SceneIcon;

		std::filesystem::path m_CurrentDirectory;
	};
}