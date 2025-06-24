#pragma once

#include <string>
#include "Core/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Snow
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& value) override;
		void UploadUniformMat3(const std::string& name, const glm::mat3& value) override;
		
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) override;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
		void UploadUniformFloat(const std::string& name, const float& value) override;
		
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value) override;
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value) override;
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value) override;
		void UploadUniformInt(const std::string& name, const int& value);

		void UploadUniformBool(const std::string& name, const bool& value);
	private:
		uint32_t m_RendererID;
	};
};