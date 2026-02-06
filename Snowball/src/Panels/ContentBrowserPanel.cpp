#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Snow
{

	ContentBrowserPanel::ContentBrowserPanel()
	: m_CurrentDirectory() 
	{
		TextureParameters parameters(TextureFormat::RGBA);
		parameters.MinFilter = SNOW_TEXTURE_LINEAR;
		parameters.MagFilter = SNOW_TEXTURE_LINEAR;
		
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/computer-folder-open-icon.png", parameters);
		m_FileIcon = Texture2D::Create("Resources/Icons/file-line-icon.png", parameters);
		m_SceneIcon = Texture2D::Create("Resources/Icons/Snowball-logo.png", parameters);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (m_AssetsPath.empty())
			return;
		ImGui::Begin("Content Browser");



		//Return arrow
		if(m_CurrentDirectory != m_AssetsPath)
		{
			if(ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1) 
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);


		for(auto& it : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = it.path();
			auto relativePath = std::filesystem::relative(path, m_AssetsPath);
			std::string filenameString = relativePath.filename().string();

			Ref<Texture2D> icon;
		
			if (!it.is_directory())
			{
				if (path.extension() == ".snow")
					icon = m_SceneIcon;
				else
					icon = m_FileIcon;
			}
			else
				icon = m_DirectoryIcon;


			ImGui::ImageButton(filenameString.c_str(),(ImTextureID)icon->GetRendererID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropSource())
			{
				auto itemPath = Snow::Utils::ToUTF8String(relativePath);

				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath.c_str(), itemPath.size()+1, ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}
			

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if(it.is_directory())
					m_CurrentDirectory /= it.path().filename();
			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::End();
	}

};
