#pragma once

#include <cstdint>

#include "AssetImportSettings.h"
#include "Core/Audio/AudioAssets.h"
#include "Core/Renderer/Texture.h"
#include "Utilities/UUID.h"
namespace Snow
{
	using AssetID = UUID;

	using TextureHandle = Ref<Texture2D>;
	using AudioHandle = Ref<AudioAsset>;
	
	enum class AssetType
	{
		File = 0, // Default for everything that i don't use
		Texture2D,
		Audio,
		Scene,
		//Shader,
		//Script,
		//Spritesheet
	};

	struct AssetEntry 
	{ 
		AssetID id;
		AssetType type;
		Path sourcePath;

		ImportSettings settings;
	};

	
};