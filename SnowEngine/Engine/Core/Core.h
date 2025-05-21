#pragma once

#include <iostream>
#include <memory>

#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../SnowEngineAPI.h"

namespace SnowEngine
{
	class SNOW_API Application
	{
	public:
		Application();
		~Application();
		void innit();
		void test();
	private:
		void cleanup();
	};
}