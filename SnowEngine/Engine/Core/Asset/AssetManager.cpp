#include "AssetManager.h"

#include "stb_image.h"

namespace Snow
{
	std::unordered_map<AssetID, AssetEntry> AssetManager::m_AssetTable;

	std::vector<char> AssetManager::m_Data;
	uint64_t AssetManager::m_dataOffset;

	std::unordered_map<AssetID, std::weak_ptr<Texture2D>> AssetManager::m_CachedTextures;

	AssetManager::AssetManager()
	{
	}

	void AssetManager::ClearCache()
	{
		m_CachedTextures.clear();
	}

	bool AssetManager::LoadPack(const std::string& filepath)
	{
		//TODO
		return true;
	}

	TextureHandle AssetManager::GetTextureHandle(AssetID id)
	{
		{
			// Look for cached texture
			auto it = m_CachedTextures.find(id);
			if (it != m_CachedTextures.end())
			{
				if(auto tex = it->second.lock())
				return tex;
			}
		}
		// if none found load it

		auto it = m_AssetTable.find(id);
		if(it == m_AssetTable.end())
			return nullptr; // TODO : return fallback texture (pink one or idk)

		const AssetEntry& entry = it->second;

		TextureHandle handle = CreateTextureFromMemory(m_AssetTable[id]);
		m_CachedTextures[id] = handle;

		return handle;
	}


	TextureHandle AssetManager::CreateTextureFromMemory(const AssetEntry& entry)
	{
		auto fileData = (const uint8_t*)(m_Data.data() + (entry.offset - m_dataOffset));

		int width, height, channels;
		stbi_uc* pixels = stbi_load_from_memory(
			fileData,
			(int)entry.size,
			&width,
			&height,
			&channels,
			0
		);
		
		TextureFormat format;
		switch(channels)
		{
		case 3:
			format = TextureFormat::RGB;
			break;
		case 4:
			format = TextureFormat::RGBA;
			break;
		default:
			format = TextureFormat::RGBA;
			break;
		}

		TextureParameters params(format);

		TextureHandle texHandle = Texture2D::Create(width, height,params);

		texHandle->SetData(pixels, (uint32_t)width * height * channels);
		
		stbi_image_free(pixels);

		return texHandle;
	}
};