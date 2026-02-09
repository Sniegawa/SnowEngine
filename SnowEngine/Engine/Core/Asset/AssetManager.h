#pragma once

#include "AssetType.h"
#include <SnowEngineAPI.h>

#include <unordered_map>
#include <string>


namespace Snow
{

	class AssetManager
	{
	public:
		static TextureHandle GetTextureHandle(AssetID id);
		static const Path GetScenePath(AssetID id);

		static void ClearCache();

		static void LoadAssets(const Path& AssetsFolderPath);

		static bool TryGetAssetID(const Path& sourcePath, AssetID& outID);

		static const AssetType& GetAssetType(const UUID& id) { return m_AssetTable[id].type; } // TODO : put this in cpp file and add error handling

	private:
		AssetManager();
		static TextureHandle CreateTextureFromMemory(const AssetEntry& entry);
	private:

		static std::unordered_map<AssetID, AssetEntry> m_AssetTable;

		static std::unordered_map<AssetID, std::weak_ptr<Texture2D>> m_CachedTextures;
	};
};