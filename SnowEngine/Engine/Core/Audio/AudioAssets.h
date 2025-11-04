#pragma once

#include <string>
#include <filesystem>


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
		std::filesystem::path filePath;
		AudioConfig defaultConfig;
		AudioType audioType;

		AudioAsset(const std::string& path, AudioType type = AudioType::SFX, const AudioConfig& config = AudioConfig())
			: filePath(path), defaultConfig(config), audioType(type) {
		}

		AudioAsset()
			: filePath(""), defaultConfig(), audioType(AudioType::SFX) {
		}
	};
};
