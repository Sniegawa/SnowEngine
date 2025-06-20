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
		m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent, 1));

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout =
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"}
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };

		std::shared_ptr<IndexBuffer> indexBuffer( IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
		#version 330 core
				
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		out vec4 v_color;
		void main()
		{
			gl_Position = vec4(a_Position,1.0);
			v_color = a_Color;
		}

		)";

		std::string fragmentSrc = R"(
			#version 330 core
		
			in vec4 v_color;
			out vec4 color;

			void main()
			{
				color = v_color;
			}
		

		)";

		m_Shader.reset(new Shader(vertexSrc,fragmentSrc));

		m_SquareVA.reset(VertexArray::Create());

		float verticesSquare[4 * (3 + 4)] = {
			-1.0f,-1.0f,0.0f,		0.0f,0.0f,0.0f,1.0f,
			1.0f,-1.0f,0.0f,		1.0f,0.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,			1.0f,1.0f,0.0f,1.0f,
			-1.0f,1.0f,0.0f,		0.0f,1.0f,0.0f,1.0f
		};

		std::shared_ptr<VertexBuffer> vb(VertexBuffer::Create(verticesSquare,sizeof(verticesSquare)));

		BufferLayout Squarelayout =
		{
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"}
		};

		vb->SetLayout(Squarelayout);

		m_SquareVA->AddVertexBuffer(vb);

		uint32_t indicesSquare[6] = {0,1,2,2,3,0};
		std::shared_ptr<IndexBuffer> ib(IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(ib);



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
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
			m_VertexArray->Bind(); 
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
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