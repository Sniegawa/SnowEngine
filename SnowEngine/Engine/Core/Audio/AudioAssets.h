#pragma once

#include <string>
#include <filesystem>

#include <SnowEngineAPI.h>

namespace Snow
{
	enum class AttenuationModel
	{
		Linear,
		Inverse,
		Exponential
	};

	enum class AudioType
	{
		SFX,
		Music
	};

	struct AudioConfig
	{
		AudioType type = AudioType::SFX;
		
		bool loop = false;
		float volume = 0.5f;
		float pitch = 1.0f;

		float minDistance = 1.0f;
		float maxDistance = 100.0f;
		AttenuationModel attenuation = AttenuationModel::Linear;
	};

	struct AudioAsset
	{
		Path filePath;
		AudioConfig defaultConfig;

		AudioAsset(const Path& path, const AudioConfig& config = AudioConfig())
			: filePath(path), defaultConfig(config){
		}

		AudioAsset()
			: filePath(""), defaultConfig() {
		}
	};
};
