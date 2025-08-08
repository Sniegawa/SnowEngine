#pragma once

#include <string>
#include <vector>

namespace Snow
{
	struct SoundAsset
	{
		// For now i'll load the sound from path as i instantiate it
		std::string filePath; 
		SoundAsset(const std::string& path) : filePath(path) {}
		SoundAsset() : filePath("") {}
	};
};