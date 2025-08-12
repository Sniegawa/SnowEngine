#pragma once

#include <string>

namespace Snow
{
	struct MusicAsset
	{
		std::string filePath;
		MusicAsset(const std::string& path) : filePath(path) {}
		MusicAsset() : filePath("") {}
	};
};