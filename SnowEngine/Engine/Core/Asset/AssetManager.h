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
		static AudioHandle GetAudioAsset(AssetID id);

		static void ClearCache();

		static void LoadAssets(const std::unordered_map<AssetID, AssetEntry>& AssetTable) { m_AssetTable = AssetTable;}

		static bool TryGetAssetID(const Path& sourcePath, AssetID& outID);

		static const AssetType& GetAssetType(const UUID& id) { return m_AssetTable[id].type; } // TODO : put this in cpp file and add error handling
		static std::string GetAssetFilename(const UUID& id) { return m_AssetTable[id].sourcePath.filename().string(); }
	private:
		AssetManager();
		static TextureHandle CreateTextureFromMemory(const AssetEntry& entry);
		static AudioHandle CreateAudioHandle(const AssetEntry& entry);
	private:

		static std::unordered_map<AssetID, AssetEntry> m_AssetTable;

		static std::unordered_map<AssetID, std::weak_ptr<Texture2D>> m_CachedTextures;
		static std::unordered_map<AssetID, std::weak_ptr<AudioAsset>> m_CachedAudio;
	};
};