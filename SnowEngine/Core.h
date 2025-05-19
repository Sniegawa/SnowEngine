#pragma once

#include <iostream>


#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace SnowEngine
{
	class Application
	{
	public:
		Application();
		~Application();
		int run();
	private:
		void initWindow();
		void initOpenGL();
		void cleanup();
		GLFWwindow* window;
	};
}