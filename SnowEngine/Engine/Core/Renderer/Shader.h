#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Snow
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name,const glm::mat4& value);
		void UploadUniformMat3(const std::string& name,const glm::mat3& value);

		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat(const std::string& name, const float& value);

		void UploadUniformInt4(const std::string& name, const glm::ivec4& value);
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value);
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value);
		void UploadUniformInt(const std::string& name, const int& value);

		void UploadUniformBool(const std::string& name, const bool& value);
	private:
		uint32_t m_RendererID;
	};
};