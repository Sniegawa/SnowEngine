#pragma once

#include <glm/glm.hpp>
#include "Camera/Camera.h"
#include "SnowEngineApi.h"
#include "Shader.h"
#include "Texture.h"

namespace Snow
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const Camera& camera);

		static void EndScene(); // End collecting data and render in batch
		
		//Primitives
		static void DrawQuad(glm::vec2 pos = glm::vec2(0.0f), const glm::vec2& size = glm::vec2(1.0f), float rotation = 0.0f, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawQuad(glm::vec3 pos = glm::vec3(0.0f), const glm::vec2& size = glm::vec2(1.0f), float rotation = 0.0f, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));

		static void Renderer2D::DrawQuad(glm::vec2 pos, const glm::vec2& size, float rotation, Ref<Texture>& texture);

		static void Renderer2D::DrawQuad(glm::vec3 pos, const glm::vec2& size, float rotation, Ref<Texture>& texture);
	};
};