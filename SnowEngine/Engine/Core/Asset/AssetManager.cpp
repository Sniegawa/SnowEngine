#include "AssetManager.h"

#include <algorithm>

#include "stb_image.h"
#include <yaml-cpp/yaml.h>
#define KEY(x) << YAML::Key << x
#define VAL(x) << YAML::Value << x
#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap
#define MAP_END   << YAML::EndMap


namespace Snow
{
	std::unordered_map<AssetID, AssetEntry> AssetManager::m_AssetTable;

	std::unordered_map<AssetID, std::weak_ptr<Texture2D>> AssetManager::m_CachedTextures;


	AssetType StringToAssetType(const std::string& typeString)
	{
		if (typeString == "Texture2D")
			return AssetType::Texture2D;
		else if (typeString == "Audio")
			return AssetType::Audio;
		else if (typeString == "Scene")
			return AssetType::Scene;
		else
		{
			SNOW_ASSERT(0, "Can't resolve Asset Type");
			return AssetType::Texture2D;
		}
	}

	std::string AssetTypeToString(AssetType type)
	{
		switch (type)
		{
		case AssetType::Texture2D:
			return "Texture2D";
		case AssetType::Audio:
			return "Audio";
		case AssetType::Scene:
			return "Scene";
		default:
			return "";
		}
	}

	bool WriteMetaFile(Path metaPath, Path sourcePath, AssetID id, AssetType* type)
	{
		std::string ext = sourcePath.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
			ext == ".bmp" || ext == ".tga" || ext == ".gif" ||
			ext == ".hdr" || ext == ".psd" || ext == ".pic" ||
			ext == ".ppm" || ext == ".pgm" || ext == ".pbm")
		{
			*type = AssetType::Texture2D;
		}
		else if(ext == ".snow")
			*type = AssetType::Scene;
		else if(ext == ".mp3" || ext == ".wav")
			*type = AssetType::Audio;
		else
		{
			return false;
		}
		
		YAML::Emitter out;
		out MAP_START;
		out KEYVAL("ID", UUIDToString(id));
		out KEYVAL("SourcePath", sourcePath.string());
		out KEYVAL("AssetType", AssetTypeToString(*type));
		out MAP_END;

		std::ofstream fout(metaPath);
		fout << out.c_str();


		return true;
	}

	AssetManager::AssetManager()
	{
	}

	void AssetManager::ClearCache()
	{
		m_CachedTextures.clear();
	}


	// TODO :
	// Update new assets on open
	// Add a way to drag and drop files to copy them and import into project
	// Use some kind of filewatcher to handle importing assets by system (not important)
	// Make engine use the Asset infrastructure
	

	void AssetManager::LoadAssets(const Path& AssetsFolderPath)
	{
		m_AssetTable.clear();
		for (auto& it : std::filesystem::recursive_directory_iterator(AssetsFolderPath))
		{

			if (!it.is_regular_file())
				continue;


			const auto& path = it.path();
			if (path.extension() == ".meta")
				continue;

			Path metaPath = path;
			metaPath += ".meta";

			AssetType type;
			AssetID id;
			Path sourcePath;
			
			if (std::filesystem::exists(metaPath))
			{
				YAML::Node data;
				try
				{
					data = YAML::LoadFile(metaPath.string());
				}
				catch (YAML::ParserException e)
				{
					SNOW_CORE_ERROR("Failed to load .snpr file '{0}'\n     {1}", metaPath.string(), e.what());
					continue;
				}
				if (!data["ID"] || !data["SourcePath"] || !data["AssetType"])
					continue;


				auto idStr = data["ID"].as<std::string>();
				id = UUIDFromString(idStr);
				sourcePath = data["SourcePath"].as<std::string>();
				type = StringToAssetType(data["AssetType"].as<std::string>());


			}
			else
			{
				id = GenerateUUID();

				bool success = WriteMetaFile(metaPath, path, id, &type);
				if (!success)
					continue;
			}
			AssetEntry entry;

			entry.id = id;
			entry.type = type;
			entry.sourcePath = path;

			m_AssetTable[id] = entry;
		}
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

		TextureHandle handle = CreateTextureFromMemory(entry);
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

		TextureParameters params(format);

		TextureHandle texHandle = Texture2D::Create(width, height,params);

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
};