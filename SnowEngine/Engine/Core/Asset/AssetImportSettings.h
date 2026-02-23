#pragma once

#include <Core/Renderer/Texture.h>
#include <Core/Audio/AudioAssets.h>
#include <variant>
namespace Snow
{
	struct FileImportSettings
	{

	};

	struct Texture2DImportSettings
	{
		TextureWrap Wrap = TextureWrap::Repeat;
		TextureFilter MinFilter = TextureFilter::Nearest;
		TextureFilter MagFilter = TextureFilter::Nearest;

		glm::vec3 Tint = glm::vec3(1.0f);
		float Opacity = 1.0f;

		Texture2DImportSettings(
			TextureWrap wrap = TextureWrap::Repeat,
			TextureFilter minFilter = TextureFilter::Nearest,
			TextureFilter magFilter = TextureFilter::Nearest,
			glm::vec3 tint = glm::vec3(1.0f),
			float opacity = 1.0f
		) : Wrap(wrap),MinFilter(minFilter),MagFilter(magFilter),Tint(tint),Opacity(opacity) {}
	};

	struct SceneImportSettings
	{
		// Idk maybe version
	};

	struct AudioImportSettings
	{
		AudioType Type = AudioType::SFX;
		AttenuationModel Attenuation = AttenuationModel::Linear;

		bool Loop = false;
		float Volume = 0.5f;
		float Pitch = 1.0f;

		float MinDistance = 1.0f;
		float MaxDistance = 100.0f;

		AudioImportSettings(
			AudioType type = AudioType::SFX,
			AttenuationModel attenuation = AttenuationModel::Linear,
			bool loop = false,
			float Volume = 0.5f,
			float pitch = 1.0f,
			float minDistance = 1.0f,
			float maxDistance = 100.0f
		) : Type(type),Attenuation(attenuation),Loop(loop),Volume(Volume),Pitch(pitch),MinDistance(minDistance),MaxDistance(maxDistance) {}
	};


	using ImportSettings = std::variant<
		FileImportSettings,
		Texture2DImportSettings,
		SceneImportSettings,
		AudioImportSettings
	>;
};