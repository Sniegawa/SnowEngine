#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Shader.h"
namespace Snow
{

	class Renderer
	{
	public:

		static void BeginScene(std::shared_ptr<Camera> camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
};