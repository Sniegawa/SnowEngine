#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : Layer("Example") , m_Camera(new Snow::OrthographicCamera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, Snow::CameraParams::OrthographicCameraParams(-1.6f, 1.6f, -0.9f, 0.9f))) 
	{

		m_VertexArray.reset(Snow::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Snow::VertexBuffer> vertexBuffer(Snow::VertexBuffer::Create(vertices, sizeof(vertices)));
		Snow::BufferLayout layout =
		{
			{Snow::ShaderDataType::Float3,"a_Position"},
			{Snow::ShaderDataType::Float4,"a_Color"}
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };

		std::shared_ptr<Snow::IndexBuffer> indexBuffer(Snow::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
		#version 330 core
				
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		
		uniform mat4 u_ViewProjection;

		out vec4 v_color;
		void main()
		{
			gl_Position = u_ViewProjection * vec4(a_Position,1.0);
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

		m_Shader.reset(new Snow::Shader(vertexSrc, fragmentSrc));

		m_SquareVA.reset(Snow::VertexArray::Create());

		float verticesSquare[4 * (3 + 4)] = {
			-1.0f,-1.0f,0.0f,		0.0f,0.0f,0.0f,1.0f,
			1.0f,-1.0f,0.0f,		1.0f,0.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,			1.0f,1.0f,0.0f,1.0f,
			-1.0f,1.0f,0.0f,		0.0f,1.0f,0.0f,1.0f
		};

		std::shared_ptr<Snow::VertexBuffer> vb(Snow::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		Snow::BufferLayout Squarelayout =
		{
			{Snow::ShaderDataType::Float3,"a_Position"},
			{Snow::ShaderDataType::Float4,"a_Color"}
		};

		vb->SetLayout(Squarelayout);

		m_SquareVA->AddVertexBuffer(vb);

		uint32_t indicesSquare[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Snow::IndexBuffer> ib(Snow::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(ib);

	}

	void OnUpdate(Snow::Timestep ts) override
	{
		float dt = ts;
		
		if (Snow::Input::IsKeyPressed(Snow::Key::Left))
		{
			m_Camera->Move(glm::vec3(-1.0f, 0.0f, 0.0f) * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::Right))
		{
			m_Camera->Move(glm::vec3(1.0f, 0.0f, 0.0f) * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::Up))
		{
			m_Camera->Move(glm::vec3(0.0f, 1.0f, 0.0f) * dt);
		}
		if (Snow::Input::IsKeyPressed(Snow::Key::Down))
		{
			m_Camera->Move(glm::vec3(0.0f, -1.0f, 0.0f) * dt);
		}

		Snow::RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.9f, 1.0f });
		Snow::RenderCommand::Clear();

		Snow::Renderer::BeginScene(m_Camera);

		m_Shader->Bind();

		Snow::Renderer::Submit(m_SquareVA, m_Shader);

		Snow::Renderer::Submit(m_VertexArray, m_Shader);

		Snow::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Snow::Event& event) override
	{
		Snow::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Snow::KeyPressedEvent>(SNOW_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent, 1));
	}

	bool OnKeyPressedEvent(Snow::KeyPressedEvent& event)
	{
		return false;
	}

private:
	std::shared_ptr<Snow::VertexArray> m_VertexArray;
	std::shared_ptr<Snow::VertexArray> m_SquareVA;

	std::shared_ptr<Snow::Shader> m_Shader;

	std::shared_ptr<Snow::Camera> m_Camera;
};

class Sandbox : public Snow::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Snow::Application* Snow::CreateApplication()
{
	return new Sandbox();
}
