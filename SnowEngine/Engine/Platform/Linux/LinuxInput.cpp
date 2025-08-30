#include <Core/Application.h>
#include <Core/Input.h>

#include <GLFW/glfw3.h>

namespace Snow
{

	bool Input::IsKeyPressed(KeyCode code)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto status = glfwGetKey(window, code);
		return status == GLFW_PRESS || status == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(MouseCode code)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto status = glfwGetMouseButton(window, code);
		return status == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return std::pair<float, float>((float)x, (float)y);
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return y;
	}
};