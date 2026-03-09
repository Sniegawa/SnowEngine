#include "AssetPropertiesPanel.h"

#include <Core/Asset/AssetManager.h>
#include <Core/Asset/AssetUtils.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Snow
{
	void AssetPropertiesPanel::Initialize(ProjectManager* PMP)
	{
		m_ProjectManager = PMP;
	}

	void AssetPropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Asset Properties");

		if(!m_selectedAssetID)
		{
			ImGui::End();
			return;
		}

		auto AssetName = AssetManager::GetAssetFilename(m_selectedAssetID);
		ImGui::Text(AssetName.c_str());

		//DEBUG
		auto idstr = UUIDToString(m_selectedAssetID);
		ImGui::Text(idstr.c_str());

		AssetEntry entry = AssetManager::GetAssetEntry(m_selectedAssetID);

		auto& settings = entry.settings;

		bool isDirty = false;

		switch(entry.type)
		{
		case AssetType::Texture2D:
			{
			auto& tis = std::get<Texture2DImportSettings>(settings);
			isDirty = DisplayTexture2DProperties(tis);
			break;
			}
		case AssetType::Audio:
			{
			auto& ais = std::get<AudioImportSettings>(settings);
			isDirty = DisplayAudioProperties(ais);
			break;
			}
		case AssetType::Scene:
			break;
		case AssetType::File:
			break;
		default:
			break;
		}
		
		if(isDirty)
			m_ProjectManager->MarkAssetDirty(entry);

		ImGui::End();
	}

	bool AssetPropertiesPanel::DisplayTexture2DProperties(Texture2DImportSettings& settings)
	{
		bool Dirty = false;

		if (ImGui::DragFloat("Opacity", &settings.Opacity, 0.02f, 0.0f, 1.0f)) Dirty = true;
		if (ImGui::DragFloat3("Tint", glm::value_ptr(settings.Tint), 0.02f, 0.0f, 1.0f)) Dirty = true;

		const char* formats[] = { "RGBA", "RGB" };
		int format = (int)settings.Format;
		if (ImGui::Combo("Format", &format, formats, IM_ARRAYSIZE(formats)))
		{
			settings.Format = (TextureFormat)format;
			Dirty = true;
		}

		const char* filters[] = { "Nearest", "Linear" };
		int minfilter = (int)settings.MinFilter;
		if (ImGui::Combo("Minification Filter", &minfilter, filters, IM_ARRAYSIZE(filters)))
		{
			settings.MinFilter = (TextureFilter)minfilter;
			Dirty = true;
		}

		int Magfilter = (int)settings.MagFilter;
		if (ImGui::Combo("Magnification Filter", &Magfilter, filters, IM_ARRAYSIZE(filters)))
		{
			settings.MagFilter = (TextureFilter)Magfilter;
			Dirty = true;
		}

		const char* wraps[] = { "Clamp", "Repeat" };
		int wrap = (int)settings.Wrap;
		if (ImGui::Combo("Wrap", &wrap, wraps, IM_ARRAYSIZE(wraps)))
		{
			settings.Wrap = (TextureWrap)wrap;
			Dirty = true;
		}

		return Dirty;
	}

	bool AssetPropertiesPanel::DisplayAudioProperties(AudioImportSettings& settings)
	{
		return false;
	}
};