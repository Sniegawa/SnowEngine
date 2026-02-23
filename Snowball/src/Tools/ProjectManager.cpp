#include "ProjectManager.h"

#include <fstream>

#include <Core/Asset/AssetType.h>
#include <Core/Asset/AssetManager.h>
#include <Core/Logging/Log.h>
#include <SnowEngineAPI.h>

#include <yaml-cpp/yaml.h>
#include "Utilities/YAML_GLM.h"


#define ASSET_SYSTEM_VERSION 0.01

namespace Snow
{
	namespace AssetUtils
	{
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
	};

	ProjectManager::ProjectManager() : m_ProjectPath(), m_ProjectName("New Project"), m_AssetsPath(), m_ProjectFilePath() {}

	ProjectManager::~ProjectManager() {}

	void ProjectManager::SaveProject(Path& LastOpenedScene)
	{
		Path ProjectDirectoryPath = m_ProjectPath;		
		Path AssetsPath = m_AssetsPath;

		YAML::Emitter out;

		out MAP_START;
		out KEYVAL("SNPR", m_ProjectName);
		out KEYVAL("VERSION", 0.01);
		out KEYVAL("ASSETS_PATH", AssetsPath.string());
		out KEYVAL("LAST_SCENE", LastOpenedScene.string());
		out MAP_END;

		std::ofstream fout(m_ProjectFilePath);
		fout << out.c_str();
	}

	bool ProjectManager::OpenProject(Path path, Path& SceneToOpen)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			SNOW_CORE_ERROR("Failed to load .snpr file '{0}'\n     {1}", path.string(), e.what());
			return false;
		}
		if (!data["SNPR"])
			return false;

		m_ProjectName = data["SNPR"].as<std::string>();

		std::string AssetsPath = data["ASSETS_PATH"].as<std::string>();

		m_AssetsPath = AssetsPath;
		m_ProjectPath = path.parent_path();

		if (data["LAST_SCENE"])
			SceneToOpen = data["LAST_SCENE"].as<std::string>();

		m_ProjectFilePath = path;
		return true;
	}

	bool ProjectManager::CreateProject(Path path, std::string name)
	{
		Path ProjectDirectoryPath = path.parent_path();
		Path AssetsPath = ProjectDirectoryPath / "Assets";
		std::filesystem::create_directory(AssetsPath);
		YAML::Emitter out;

		out MAP_START;
		out KEYVAL("SNPR", name);
		out KEYVAL("VERSION", 0.01);
		out KEYVAL("ASSETS_PATH", AssetsPath.string());
		out KEYVAL("LAST_SCENE", "");
		out MAP_END;

		std::ofstream fout(path);
		fout << out.c_str();

		m_ProjectName = name;
		m_ProjectPath = ProjectDirectoryPath;
		m_AssetsPath = AssetsPath;
		m_ProjectFilePath = path;
		return true;
	}

	void WriteAssetImportSettings(YAML::Emitter& out, ImportSettings settings)
	{
		out KEY("ImportSettings");
		out MAP_START;

		std::visit([&](auto&& s) 
			{
				using T = std::decay_t<decltype(s)>; // Get the type of settings

				if constexpr (std::is_same_v<T, Texture2DImportSettings>)
				{
					out KEYVAL("Wrap", (int)s.Wrap);
					out KEYVAL("MinFilter", (int)s.MinFilter);
					out KEYVAL("MagFilter", (int)s.MagFilter);
					out KEYVAL("Tint", s.Tint);
					out KEYVAL("Opacity", s.Opacity);
				}
				else if constexpr (std::is_same_v<T,AudioImportSettings>)
				{
					out KEYVAL("Type", (int)s.Type);
					out KEYVAL("Attenuation", (int)s.Attenuation);
					out KEYVAL("Loop", s.Loop);
					out KEYVAL("Volume", s.Volume);
					out KEYVAL("Pitch", s.Pitch);
					out KEYVAL("MinDistance", s.MinDistance);
					out KEYVAL("MaxDistance", s.MaxDistance);
				}

			}, settings);

		out MAP_END;
	}

	void CreateDirectoryMetaFile(const Path& metaPath, const Path& sourcePath, UUID id)
	{
		YAML::Emitter out;
		out MAP_START;
		out KEYVAL("ID", UUIDToString(id));
		out KEYVAL("Version", ASSET_SYSTEM_VERSION);
		out KEYVAL("DirectoryPath", sourcePath.string());
		out MAP_END;

		std::ofstream fout(metaPath);
		fout << out.c_str();
	}

	void CreateNewMetaFile(const Path& metaPath, const Path& sourcePath, AssetID id)
	{
		AssetType type;
		std::string ext = sourcePath.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
			ext == ".bmp" || ext == ".tga" || ext == ".gif" ||
			ext == ".hdr" || ext == ".psd" || ext == ".pic" ||
			ext == ".ppm" || ext == ".pgm" || ext == ".pbm")
		{
			type = AssetType::Texture2D;
		}
		else if (ext == ".snow")
			type = AssetType::Scene;
		else if (ext == ".mp3" || ext == ".wav")
			type = AssetType::Audio;
		else
			type = AssetType::File;

		ImportSettings settings;

		switch (type)
		{
		case AssetType::Texture2D:
			settings = Texture2DImportSettings();
			break;
		case AssetType::Audio:
			settings = AudioImportSettings();
			break;
		case AssetType::Scene:
			settings = SceneImportSettings();
			break;
		case AssetType::File:
			settings = FileImportSettings();
			break;
		default:
			SNOW_CORE_ERROR("COULD NOT RESOLVE DEFAULT ASSET IMPORT SETTINGS FOR ASSET TYPE {0}",AssetUtils::AssetTypeToString(type));
			break;
		}

		YAML::Emitter out;
		out MAP_START;
		out KEYVAL("ID", UUIDToString(id));
		out KEYVAL("Version", ASSET_SYSTEM_VERSION);
		out KEYVAL("SourcePath", sourcePath.string());
		out KEYVAL("AssetType", AssetUtils::AssetTypeToString(type));

		WriteAssetImportSettings(out, settings);

		out MAP_END;

		std::ofstream fout(metaPath);
		fout << out.c_str();
	}

	ImportSettings GetImportSettings(YAML::Node& data, AssetType type)
	{
		auto node = data["ImportSettings"];
		switch(type)
		{
			case AssetType::Texture2D:
			{
				Texture2DImportSettings s;

				s.Wrap = (TextureWrap)node["Wrap"].as<int>();
				s.MinFilter = (TextureFilter)node["MinFilter"].as<int>();
				s.MagFilter = (TextureFilter)node["MagFilter"].as<int>();
				s.Tint = node["Tint"].as<glm::vec3>();
				s.Opacity = node["Opacity"].as<float>();

				return s;
			}
			case AssetType::Audio:
			{
				AudioImportSettings s;

				AudioType Type = AudioType::SFX;
				AttenuationModel Attenuation = AttenuationModel::Linear;

				s.Type = (AudioType)node["Type"].as<int>();
				s.Attenuation = (AttenuationModel)node["Attenuation"].as<int>();
				s.Loop = node["Loop"].as<bool>();
				s.Volume = node["Volume"].as<float>();
				s.Pitch = node["Pitch"].as<float>();
				s.MinDistance = node["MinDistance"].as<float>();
				s.MaxDistance = node["MaxDistance"].as<float>();

				return s;
			}
			default:
				return FileImportSettings();
		}
	}

	void ProjectManager::LoadAssets(const Path& AssetsFolderPath)
	{
		std::unordered_map<AssetID, AssetEntry> AssetTable;

		// First look up what assets miss .meta files
		std::set<Path> AssetsToLoad;
		std::set<Path> AssetsWithoutMeta;
		std::set<Path> DirectoryMetaFiles;


		for (auto& it : std::filesystem::recursive_directory_iterator(AssetsFolderPath))
		{
			const auto& path = it.path();
			Path metaPath = path;
			metaPath += ".meta";

			if (it.is_directory())
			{
				if (!std::filesystem::exists(metaPath))
				{
					UUID id = GenerateUUID();
					CreateDirectoryMetaFile(path, metaPath, id);
				}
				else // Check if meta file has correct version
				{
					YAML::Node data;
					try
					{
						data = YAML::LoadFile(metaPath.string());
					}
					catch (YAML::ParserException e)
					{
						SNOW_CORE_WARN("Failed to load .meta file '{0}'\n     {1}", metaPath.string(), e.what());
						continue;
					}
					if (data["Version"].as<double>() != ASSET_SYSTEM_VERSION)
					{
						auto idStr = data["ID"].as<std::string>();
						auto id = UUIDFromString(idStr);
						CreateDirectoryMetaFile(path, metaPath, id);
					}
				}

				DirectoryMetaFiles.insert(metaPath);
				continue;
			}

			if (!it.is_regular_file())
				continue;

			if (path.extension() == ".meta")
				continue;

			if (std::filesystem::exists(metaPath))
				AssetsToLoad.insert(path);
			else
				AssetsWithoutMeta.insert(path);

		}

		for (const auto& path : AssetsWithoutMeta)
		{
			auto id = GenerateUUID();

			Path metaPath = path;
			metaPath += ".meta";

			CreateNewMetaFile(metaPath, path, id);
			AssetsToLoad.insert(path);
		}

		for (const auto& AssetPath : AssetsToLoad)
		{
			Path metaPath = AssetPath;
			metaPath += ".meta";

			AssetType type;
			AssetID id;
			Path sourcePath;

			YAML::Node data;
			try
			{
				data = YAML::LoadFile(metaPath.string());
			}
			catch (YAML::ParserException e)
			{
				SNOW_CORE_WARN("Failed to load .meta file '{0}'\n     {1}", metaPath.string(), e.what());
				continue;
			}


			if (!data["ID"] || !data["SourcePath"] || !data["AssetType"])
				continue;

			if (!data["Version"] || data["Version"].as<double>() != ASSET_SYSTEM_VERSION)
			{
				// Handle different versions
			}

			auto idStr = data["ID"].as<std::string>();
			id = UUIDFromString(idStr);
			sourcePath = data["SourcePath"].as<std::string>();
			type = AssetUtils::StringToAssetType(data["AssetType"].as<std::string>());

			ImportSettings settings = GetImportSettings(data,type);

			AssetEntry entry;

			entry.id = id;
			entry.type = type;
			entry.sourcePath = sourcePath;
			entry.settings = settings;
			AssetTable[id] = entry;
		}

		AssetManager::LoadAssets(AssetTable);
	}

}