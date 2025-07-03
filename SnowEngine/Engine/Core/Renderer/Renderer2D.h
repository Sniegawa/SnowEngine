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
		static void Flush();
		
		//Primitives
		static void DrawQuad(glm::vec2 pos, const glm::vec2& size , const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawQuad(glm::vec3 pos, const glm::vec2& size, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));

		static void DrawQuad(glm::vec2 pos, const glm::vec2& size, Ref<Texture2D>& texture);
		static void DrawQuad(glm::vec3 pos, const glm::vec2& size, Ref<Texture2D>& texture);
		
		static void DrawRotatedQuad(glm::vec2 pos, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawRotatedQuad(glm::vec3 pos, const glm::vec2& size, float rotation,const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawRotatedQuad(glm::vec2 pos, const glm::vec2& size,float rotation, Ref<Texture2D>& texture);
		static void DrawRotatedQuad(glm::vec3 pos, const glm::vec2& size,float rotation, Ref<Texture2D>& texture);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
		
	};
};