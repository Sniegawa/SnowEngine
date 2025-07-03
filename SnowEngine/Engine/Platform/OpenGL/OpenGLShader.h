#pragma once

#include <string>
#include "Core/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Snow
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment, bool isFile = 0);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) override;
		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& value) override;
		
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void UploadUniformFloat(const std::string& name, const float& value) override;
		
		virtual void UploadUniformInt4(const std::string& name, const glm::ivec4& value) override;
		virtual void UploadUniformInt3(const std::string& name, const glm::ivec3& value) override;
		virtual void UploadUniformInt2(const std::string& name, const glm::ivec2& value) override;
		virtual void UploadUniformInt(const std::string& name, const int& value);
		virtual void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void UploadUniformBool(const std::string& name, const bool& value);
	private:
		std::string m_Name;
		uint32_t m_RendererID;
	};
};