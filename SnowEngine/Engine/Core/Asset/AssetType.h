#pragma once

#include <cstdint>

#include "Core/Renderer/Texture.h"
#include "Core/Audio/AudioAssets.h"

namespace Snow
{
	using AssetID = uint64_t;

	using TextureHandle = Ref<Texture2D>;
	
	enum class AssetType
	{
		Texture2D = 0,
		Shader,
		//Audio,
		//Script,
		//Spritesheet
	};

	struct AssetEntry { AssetType type; uint64_t offset, size; };

};