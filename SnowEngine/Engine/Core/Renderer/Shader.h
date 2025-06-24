#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Snow
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& value) = 0;

		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void UploadUniformFloat(const std::string& name, const float& value) = 0;

		virtual void UploadUniformInt4(const std::string& name, const glm::ivec4& value) = 0;
		virtual void UploadUniformInt3(const std::string& name, const glm::ivec3& value) = 0;
		virtual void UploadUniformInt2(const std::string& name, const glm::ivec2& value) = 0;
		virtual void UploadUniformInt(const std::string& name, const int& value) = 0;

		virtual void UploadUniformBool(const std::string& name, const bool& value) = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};
};