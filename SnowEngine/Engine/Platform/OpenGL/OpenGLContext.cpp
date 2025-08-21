#include "OpenGLContext.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SnowEngineAPI.h>
#include "SnowEngine/Engine/Core/Logging/Log.h"

Snow::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	SNOW_CORE_ASSERT(windowHandle, "Window handle is null");
}

void Snow::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	SNOW_CORE_ASSERT(status, "Failed to initialize GLAD!");

	SNOW_CORE_INFO("OpenGL Info:");
	SNOW_CORE_INFO(" Vendor : {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	SNOW_CORE_INFO(" Renderer : {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	SNOW_CORE_INFO(" Version : {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	int maxtex;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxtex);

	SNOW_CORE_INFO("Max texture slots : {0}", maxtex);
}

void Snow::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
