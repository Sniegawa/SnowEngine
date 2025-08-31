#pragma once

#include <glm/glm.hpp>

#include "Core/Renderer/Camera/Camera.h"

namespace Snow
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default; //Default copy constructor
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		operator std::string() { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default; //Default copy constructor
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) { }

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f,1.0f,1.0f,1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default; //Default copy constructor
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) { }

		operator glm::vec4& () { return Color; }
		operator const glm::vec4& () { return Color; }
	};

	struct CameraComponent
	{
		Camera Cam;
		bool Primary = false;


		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection, bool primary = false)
			: Cam(projection), Primary(primary) { }

	};
}