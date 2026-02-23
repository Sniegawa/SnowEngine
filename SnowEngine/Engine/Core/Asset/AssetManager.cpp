#include "AssetManager.h"

#include <algorithm>
#include <set>

#include "stb_image.h"
#include <yaml-cpp/yaml.h>

#define KEY(x) << YAML::Key << x
#define VAL(x) << YAML::Value << x
#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap
#define MAP_END   << YAML::EndMap


/* TODO LIST
*	- PUT ALL OF THAT IN PROJECT MANAGER OR SOMETGHING LIKE THAT ON THE EDITOR SIDE
*	- A way to add assets during runtime of engine
*	- Spritesheets
* 
*	- File watcher :
*		- When a file gets renamed during engine runtime, it detects it and updates the meta file with this new information
*		- When a file gets added during engine runtime, it creates a meta file for it
*		- When a file gets deleted during engine runtime, it should do something with it (delete the asset from memory or idk)
*		- Handle all above but for directories
*/


namespace Snow
{
	std::unordered_map<AssetID, AssetEntry> AssetManager::m_AssetTable;

	std::unordered_map<AssetID, std::weak_ptr<Texture2D>> AssetManager::m_CachedTextures;
	std::unordered_map<AssetID, std::weak_ptr<AudioAsset>> AssetManager::m_CachedAudio;

	void AssetManager::ClearCache()
	{
		m_CachedTextures.clear();
	}

	const Path AssetManager::GetScenePath(AssetID id)
	{
		auto it = m_AssetTable.find(id);
		if (it != m_AssetTable.end())
			return it->second.sourcePath;
		else
			return "";
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
		if(entry.type != AssetType::Texture2D)
		{
			SNOW_CORE_WARN("Tried to get a texture with uuid not bound to texture {0}", UUIDToString(id));
			return nullptr;
		}

		TextureHandle handle = CreateTextureFromMemory(entry);
		SNOW_CORE_TRACE("Loaded Texture2D asset from memory; ID = {0}", UUIDToString(id));
		m_CachedTextures[id] = handle;

		return handle;
	}


	TextureHandle AssetManager::CreateTextureFromMemory(const AssetEntry& entry)
	{
		int width, height, channels;
		stbi_uc* pixels = stbi_load(
			entry.sourcePath.string().c_str(),
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

		Texture2DImportSettings settings = std::get<Texture2DImportSettings>(entry.settings);

		TextureParameters params(format);

		params.Wrap = settings.Wrap;
		params.MinFilter = settings.MinFilter;
		params.MagFilter = settings.MagFilter;

		TextureHandle texHandle = Texture2D::Create(width, height, params);

		texHandle->SetTextureTint(settings.Tint);
		texHandle->SetTextureOpacity(settings.Opacity);

		texHandle->SetData(pixels, (uint32_t)width * height * channels);
		
		stbi_image_free(pixels);

		return texHandle;
	}

	bool AssetManager::TryGetAssetID(const Path& sourcePath, AssetID& outID)
	{
		for(const auto& [id,entry] : m_AssetTable)
		{
			if(std::filesystem::equivalent(entry.sourcePath,sourcePath))
			{
				outID = id;
				return true;
			}
		}
		return false;
	}

	AudioHandle AssetManager::CreateAudioHandle(const AssetEntry& entry)
	{
		AudioImportSettings settings = std::get<AudioImportSettings>(entry.settings);

		AudioConfig config;
		config.attenuation = settings.Attenuation;
		config.loop = settings.Loop;
		config.minDistance = settings.MinDistance;
		config.maxDistance = settings.MaxDistance;
		config.pitch = settings.Pitch;
		config.type = settings.Type;
		config.volume = settings.Volume;

		const Path path = entry.sourcePath;

		AudioHandle handle = CreateRef<AudioAsset>(path, config);

		return handle;

	}

	AudioHandle AssetManager::GetAudioAsset(AssetID id)
	{
		{
			// Look for cached texture
			auto it = m_CachedAudio.find(id);
			if (it != m_CachedAudio.end())
			{
				if (auto audio = it->second.lock())
					return audio;
			}
		}
		// if none found load it

		auto it = m_AssetTable.find(id);
		if (it == m_AssetTable.end())
		{
			SNOW_CORE_WARN("Couldn't find an audio asset with given ID : {0}", UUIDToString(id));
			return nullptr;
		}
		const AssetEntry& entry = it->second;
		if (entry.type != AssetType::Audio)
		{
			SNOW_CORE_WARN("Tried to get a audio with uuid not bound to audio {0}", UUIDToString(id));
			return nullptr;
		}

		AudioHandle handle = CreateAudioHandle(entry);
		SNOW_CORE_TRACE("Loaded audio asset from memory; ID = {0}", UUIDToString(id));
		m_CachedAudio[id] = handle;

		return handle;
	}

	AssetManager::AssetManager() {}
};