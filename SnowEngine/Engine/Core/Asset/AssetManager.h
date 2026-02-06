#pragma once

#include "AssetType.h"

#include <unordered_map>
#include <string>

namespace Snow
{

	class AssetManager
	{
	public:
		static bool LoadPack(const std::string& filepath);

		static TextureHandle GetTextureHandle(AssetID id);

		static void ClearCache();
	private:
		AssetManager();
		static TextureHandle CreateTextureFromMemory(const AssetEntry& entry);
	private:

		static std::unordered_map<AssetID, AssetEntry> m_AssetTable;

		static std::vector<char> m_Data;
		static uint64_t m_dataOffset;

		static std::unordered_map<AssetID, std::weak_ptr<Texture2D>> m_CachedTextures;
	};
};