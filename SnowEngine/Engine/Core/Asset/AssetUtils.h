#pragma once

#include <string>
#include "AssetType.h"
#include "Core/Audio/AudioAssets.h"
#include "Core/Renderer/Texture.h"
#include "AssetImportSettings.h"

namespace Snow
{
	namespace AssetUtils
	{
		AssetType StringToAssetType(const ::std::string& typeString);

		std::string AssetTypeToString(AssetType type);

		TextureParameters ImportSettingsToParameters(const Texture2DImportSettings& settings);

    AudioConfig ImportSettingsToAudioConfig(const AudioImportSettings& settings);

	};
}
