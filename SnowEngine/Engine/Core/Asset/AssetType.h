#pragma once

#include <cstdint>

#include "Core/Renderer/Texture.h"
#include "Core/Audio/AudioAssets.h"
#include "Utilities/UUID.h"
namespace Snow
{
	using AssetID = UUID;

	using TextureHandle = Ref<Texture2D>;
	
	enum class AssetType
	{
		Texture2D = 0,
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
	};

};