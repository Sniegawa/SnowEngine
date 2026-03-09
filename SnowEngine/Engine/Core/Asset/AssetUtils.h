#pragma once

#include <string>
#include "AssetType.h"
#include "Core/Renderer/Texture.h"
#include "AssetImportSettings.h"

namespace Snow
{
	namespace AssetUtils
	{
		AssetType StringToAssetType(const ::std::string& typeString);

		std::string AssetTypeToString(AssetType type);

		TextureParameters ImportSettingsToParameters(const Texture2DImportSettings& settings);

	};
}