#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h";

namespace Snow
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
};