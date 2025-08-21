#pragma once

#include <string>

//I don't like the approach where i have Music and Sound structs,
//in one file, but it's the best option for now to ensure that i 
//have Attenuation Model same for them both

namespace Snow
{
	enum class AttenuationModel
	{
		Inverse,
		Linear,
		Exponential
	};

	struct MusicConfig
	{
		float volume = 1.0f;
		float pitch = 1.0f;
		bool looping = false;
		float spatialBlend = 1.0f; // 2D->3D blend
		float minDistance = 1.0f;
		float maxDistance = 100.0f;
		AttenuationModel attenuation = AttenuationModel::Linear;
	};

	struct MusicAsset
	{
		std::string filePath;
		MusicConfig defaultConfig;

		MusicAsset(const std::string& path) : 
			filePath(path) {}

		MusicAsset(const std::string& path, MusicConfig config)
			: filePath(path), defaultConfig(config) {}

		MusicAsset() 
			: filePath(""), defaultConfig() {}
	};
	
	struct SoundConfig
	{
		float volume = 1.0f;
		float pitch = 1.0f;
		float spatialBlend = 1.0f; // 2D->3D blend
		float minDistance = 1.0f;
		float maxDistance = 100.0f;
		AttenuationModel attenuation = AttenuationModel::Linear;
	};

	struct SoundAsset
	{
		std::string filePath;
		SoundConfig defaultConfig;

		SoundAsset(const std::string& path) 
			: filePath(path) , defaultConfig(SoundConfig()) {}

		SoundAsset(const std::string& path,SoundConfig config)
			: filePath(path), defaultConfig(config) {}

		SoundAsset()
			 : filePath(""), defaultConfig(SoundConfig()) {}
	};
};