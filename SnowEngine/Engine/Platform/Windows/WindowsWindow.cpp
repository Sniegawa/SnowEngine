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

	Ref<IWindow> IWindow::Create()
	{
		return std::make_shared<WindowsWindow>();
	}

	WindowsWindow::WindowsWindow()
	{
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
			SNOW_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			SNOW_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = CreateScope<OpenGLContext>(m_Window);

		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(m_Data.VSync);

		//Set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent e(width,height);
			data.EventCallback(e);
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.EventCallback(e);
			});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
				case GLFW_PRESS:
					{
					KeyPressedEvent e(key,0);
					data.EventCallback(e);
					break;
					}
		     		case GLFW_RELEASE:
		     			{
					KeyReleasedEvent e(key);
		     			data.EventCallback(e);
		     			}
		     			break;
				case GLFW_REPEAT:
		     			{
		     			//Glfw doesn't provide repeat count, so we use 1 for simplicity, in future can fork glfw to add this feature
					KeyPressedEvent e(key,1);
		     			data.EventCallback(e);
		     			}
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
			     		{
					MouseButtonPressedEvent e(button);
			     		data.EventCallback(e);
			     		}
			     		break;
				case GLFW_RELEASE:
					{
					MouseButtonReleasedEvent e(button);	
			     		data.EventCallback(e);
					}
					break;
				default:
					SNOW_CORE_ERROR("Unknown MouseButton action: {0}", action);
					break;

				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent e((float)xoffset,(float)yoffset);
				data.EventCallback(e);
			});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				if (focused == GLFW_TRUE)
				{
					WindowFocusEvent e;
					data.EventCallback(e);
				}
				else if (focused == GLFW_FALSE)
				{
			     		WindowLostFocusEvent e;
					data.EventCallback(e);
				}
				else
				{
					SNOW_CORE_ERROR("Unknown window focus change : {0}", focused);
				}
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent e((float)xPos,(float)yPos);
				data.EventCallback(e);
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
