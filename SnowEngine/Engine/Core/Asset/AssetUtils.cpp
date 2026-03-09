#include "AssetUtils.h"
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
			else if (typeString == "File")
				return AssetType::File;
			else
			{
				SNOW_ASSERT(0, "Can't resolve Asset Type");
				return AssetType::File;
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
			case AssetType::File:
				return "File";
			default:
				SNOW_ASSERT(0, "Can't resolve Asset Type");
				return "Error";
			}
		}

		TextureParameters ImportSettingsToParameters(const Texture2DImportSettings& settings)
		{
			TextureParameters params;
			params = *(TextureParameters*)(&settings); // should work 

			return params;
		}
	};
};