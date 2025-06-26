#include "Shader.h"

#include "Renderer.h"
#include "Core/Logging/Log.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Snow
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, bool isFile)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SNOW_CORE_ERROR("Renderer API is set to none");		return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc, isFile);
		}
		SNOW_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		SNOW_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, bool isFile)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc, isFile);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SNOW_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];

	}


};