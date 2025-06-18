#include "Application.h"

//Temp
#include <glad/glad.h>

namespace Snow 
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SNOW_ASSERT(!s_Instance, "App already exists!");
		s_Instance = this;
		Log::Init();
		m_Window = IWindow::Create(WindowProperties(1280, 720, "SnowEngine"));
		m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent,1));

		m_ImGuiLayer = new ImGuiLayer();
		
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArray);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);

		std::string vertexSrc = R"(
		#version 330 core
				
		layout(location = 0) in vec3 a_Position;
	
		out vec3 v_Position;

		void main()
		{
			gl_Position = vec4(a_Position,1.0);
			v_Position = a_Position;
		}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
		
			in vec3 v_Position;
			out vec4 color;

			void main()
			{
				color = vec4(v_Position + 0.5,1.0);
			}
		

		)";

		m_Shader.reset(new Shader(vertexSrc,fragmentSrc));

		SNOW_CORE_INFO("Application initialized");
	}

	Application::~Application()
	{
		delete m_Window;
		SNOW_CORE_INFO("Application destruction");
	}

	void Application::PushLayer(Layer* layer)
	{
		SNOW_CORE_INFO("Attaching layer {0}", layer->GetName());
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();

	}

	void Application::PushOverlay(Layer* overlay)
	{
		SNOW_CORE_INFO("Attaching overlay {0}", overlay->GetName());
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SNOW_BIND_EVENT_FN(Application::OnWindowClose,1));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Run()
	{
		SNOW_CORE_INFO("Application running");

		while (!m_ShouldClose)
		{
			glClearColor(0.4f, 0.4f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

			m_Window->ClearWindow();
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();
			m_Window->OnUpdate();
			
			if (Input::IsKeyPressed(Key::A))
			{
				auto [x, y] = Input::GetMousePosition();

				SNOW_CORE_TRACE("{0},{1}", x, y);
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_ShouldClose = true;
		return true;
	}
};