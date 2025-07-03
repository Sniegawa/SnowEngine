#include "OpenGLShader.h"

#include <glad/glad.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "SnowEngine/Engine/Core/Logging/Log.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace Snow
{
	bool checkCompileErrors(unsigned int object, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				SNOW_CORE_ERROR("Shader: Compile-time error: Type: {0} \n {1}", type, infoLog);
			}

		}
		else
		{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				SNOW_CORE_ERROR("Shader: Link-time error: Type: {0} \n {1}",type,infoLog);

			}
		}
		return success;
	}

	//TODO : Support for more shader types
	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertex, const std::string& fragment, bool isFile)
	{
		m_Name = name;

		if (!isFile)
		{

			// Read our shaders into the appropriate buffers
			std::string vertexSource = vertex; // Get source code for vertex shader.
			std::string fragmentSource = fragment; // Get source code for fragment shader.

			// Create an empty vertex shader handle
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)vertexSource.c_str();
			glShaderSource(vertexShader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(vertexShader);

			GLint isCompiled = 0;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(vertexShader);

				// Use the infoLog as you see fit.
				SNOW_CORE_ERROR("Vertex shader did not compile");
				SNOW_CORE_ERROR("{0}", infoLog.data());
				// In this simple program, we'll just leave
				return;
			}

			// Create an empty fragment shader handle
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			// Send the fragment shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			source = (const GLchar*)fragmentSource.c_str();
			glShaderSource(fragmentShader, 1, &source, 0);

			// Compile the fragment shader
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(fragmentShader);
				// Either of them. Don't leak shaders.
				glDeleteShader(vertexShader);

				// Use the infoLog as you see fit.
				SNOW_CORE_ERROR("Fragment shader did not compile");
				SNOW_CORE_ERROR("{0}", infoLog.data());
				// In this simple program, we'll just leave
				return;
			}

			// Vertex and fragment shaders are successfully compiled.
			// Now time to link them together into a program.
			// Get a program object.
			m_RendererID = glCreateProgram();

			// Attach our shaders to our program
			glAttachShader(m_RendererID, vertexShader);
			glAttachShader(m_RendererID, fragmentShader);

			// Link our program
			glLinkProgram(m_RendererID);

			// Note the different functions here: glGetProgram* instead of glGetShader*.
			GLint isLinked = 0;
			glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(m_RendererID);
				// Don't leak shaders either.
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);

				// Use the infoLog as you see fit.
				SNOW_CORE_ERROR("Shader linking did not compile");
				SNOW_CORE_ERROR("{0}", infoLog.data());
				// In this simple program, we'll just leave
				return;
			}

			// Always detach shaders after a successful link.
			glDetachShader(m_RendererID, vertexShader);
			glDetachShader(m_RendererID, fragmentShader);
		}
		else
		{
			std::string vertexCode;
			std::string fragmentCode;

			try
			{
				std::ifstream vertexShaderFile(vertex);
				std::ifstream fragmentShaderFile(fragment);
				std::stringstream vShaderStream, fShaderStream;
				//read file buffer
				vShaderStream << vertexShaderFile.rdbuf();
				fShaderStream << fragmentShaderFile.rdbuf();
				//close files
				vertexShaderFile.close();
				fragmentShaderFile.close();

				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();


			}
			catch (std::exception e)
			{
				SNOW_CORE_ERROR("ERROR::SHADER: Failed to read shader files");
			}
			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			unsigned int sVertex, sFragment;

			sVertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(sVertex, 1, &vShaderCode, NULL);
			glCompileShader(sVertex);
			if (!checkCompileErrors(sVertex, "VERTEX"))
				SNOW_CORE_ERROR("Couldn't compile Vertex Shader");

			sFragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(sFragment, 1, &fShaderCode, NULL);
			glCompileShader(sFragment);
			if (!checkCompileErrors(sFragment, "FRAGMENT"))
				SNOW_CORE_ERROR("Couldn't compile Fragment Shader");

			m_RendererID = glCreateProgram();
			glAttachShader(m_RendererID, sVertex);
			glAttachShader(m_RendererID, sFragment);
			glLinkProgram(m_RendererID);
			if (!checkCompileErrors(m_RendererID, "PROGRAM"))
				SNOW_CORE_ERROR("Couldn't compile Program");
			glDeleteShader(sVertex);
			glDeleteShader(sFragment);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(m_RendererID);
	}
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformBool(const std::string& name, const bool& value)
	{
		GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
};