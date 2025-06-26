#pragma once

#include <string>

#include <glm/glm.hpp>

#include "SnowEngineAPI.h"

namespace Snow
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

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

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, bool isFile = true);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, bool isFile = true);
		Ref<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
};