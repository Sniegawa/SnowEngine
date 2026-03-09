#pragma once

#include <Core/Asset/AssetType.h>
#include <Tools/ProjectManager.h>

namespace Snow
{
	class AssetPropertiesPanel
	{
	public:
		AssetPropertiesPanel() = default;
		~AssetPropertiesPanel() = default;

		void Initialize(ProjectManager* PMP);

		void OnImGuiRender();

		void SetSelectedAsset(AssetID id) { m_selectedAssetID = id; }

	private:
		bool DisplayTexture2DProperties(Texture2DImportSettings& settings);
		bool DisplayAudioProperties(AudioImportSettings& settings);
	private:
		AssetID m_selectedAssetID = UUID();
		ProjectManager* m_ProjectManager;
	};
};