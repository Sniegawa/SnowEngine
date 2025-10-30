#pragma once

#include <glm/glm.hpp>
#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"
#include "SnowEngineAPI.h"
#include "Shader.h"
#include "Texture.h"
#include "Spritesheet.h"

namespace Snow
{


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();
		
		//Primitives
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size , const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Subtexture2D>& subTexture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, Ref<Subtexture2D>& subTexture);
		
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation,const glm::vec4& color = glm::vec4(0.8f, 0.2f, 0.7f, 1.0f));
									
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,float rotation, Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,float rotation, Ref<Texture2D>& texture);
									
		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size,float rotation, Ref<Subtexture2D>& subTexture);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size,float rotation, Ref<Subtexture2D>& subTexture);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, Ref<Texture2D>& texture);
		
		static void DrawQuad(const glm::mat4& transform, Ref<Subtexture2D>& subTexture);


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
