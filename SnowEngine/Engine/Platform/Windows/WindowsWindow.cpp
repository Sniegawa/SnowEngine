#include "WindowsWindow.h"
#include "SnowEngine/Engine/Core/Logging/Log.h"
#include "SnowEngine/Engine/Core/Events/ApplicationEvent.h"
#include "SnowEngine/Engine/Core/Events/KeyEvent.h"
#include "SnowEngine/Engine/Core/Events/MouseEvent.h"
#include "SnowEngine/Engine/Platform/OpenGL/OpenGLContext.h"


namespace Snow
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SNOW_CORE_ERROR("GLFW Error ({0}) : {1}", error, description);
	}

	IWindow* IWindow::Create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;


		SNOW_CORE_INFO("Creating window: {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			SNOW_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);

		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(m_Data.VSync);

		//Set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			data.EventCallback(WindowResizeEvent(width, height));
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.EventCallback(WindowCloseEvent());
			});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);


				switch (action)
				{
				case GLFW_PRESS:
					data.EventCallback(KeyPressedEvent(key, 0));
					break;
				case GLFW_RELEASE:
					data.EventCallback(KeyReleasedEvent(key));
					break;
				case GLFW_REPEAT:
					//Glfw doesn't p[rovide repeat count, so we use 1 for simplicity, in future can fork glfw to add this feature
					data.EventCallback(KeyPressedEvent(key, 1));
					break;
				default:
					SNOW_CORE_ERROR("Unknown key action: {0}", action);
					break;
				}

			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					data.EventCallback(MouseButtonPressedEvent(button));
					break;
				case GLFW_RELEASE:
					data.EventCallback(MouseButtonReleasedEvent(button));
					break;
				default:
					SNOW_CORE_ERROR("Unknown MouseButton action: {0}", action);
					break;

				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.EventCallback(MouseScrolledEvent((float)xoffset, (float)yoffset));
			});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				if (focused == GLFW_TRUE)
				{
					data.EventCallback(WindowFocusEvent());
				}
				else if (focused == GLFW_FALSE)
				{
					data.EventCallback(WindowLostFocusEvent());
				}
				else
				{
					SNOW_CORE_ERROR("Unknown window focus change : {0}", focused);
				}
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				data.EventCallback(MouseMovedEvent((float)xPos, (float)yPos));
			});
	}

	void WindowsWindow::Shutdown()
	{
		SNOW_CORE_INFO("Shutting down window: {0}", m_Data.Title);
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.VSync = enabled;
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		SNOW_CORE_INFO("VSync is {0}", m_Data.VSync ? "enabled" : "disabled");
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
		
	}
};