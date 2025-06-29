#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : 
		Layer("Application Example Layer"),
		m_CameraController(glm::vec3(0.0f,0.0f,0.0f),0.0f,(1280.0f/720.0f),true)
	{
		m_ShaderLib.Load("ColorPicker", "Assets/Shaders/PlainColor.vert", "Assets/Shaders/PlainColor.frag", true);
		m_ShaderLib.Load("Texture", "Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag", true);

		m_Texture = Snow::Texture2D::Create("Assets/Textures/pizza.png");

		m_SquareVA.reset(Snow::VertexArray::Create());

		float verticesSquare[4 * 5] = {
			-0.5f,-0.5f,0.0f, 0.0f, 0.0f,
			0.5f,-0.5f,0.0f, 1.0f, 0.0f,
			0.5f,0.5f,0.0f, 1.0f, 1.0f,
			-0.5f,0.5f,0.0f, 0.0f, 1.0f
		};

		Snow::Ref<Snow::VertexBuffer> vb(Snow::VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));

		Snow::BufferLayout Squarelayout =
		{
			{Snow::ShaderDataType::Float3,"a_Position"},
			{Snow::ShaderDataType::FLoat2,"a_TexCoord"}
		};

		vb->SetLayout(Squarelayout);

		m_SquareVA->AddVertexBuffer(vb);

		uint32_t indicesSquare[6] = { 0,1,2,2,3,0 };
		Snow::Ref<Snow::IndexBuffer> ib(Snow::IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(ib);

	}

	void OnUpdate(Snow::Timestep ts) override
	{

		m_CameraController.OnUpdate(ts);
		colorval += ts;
		m_Color1 = glm::vec4(sin(colorval), cos(colorval), std::max(tan(colorval), 1.0f), 1.0f);
		m_Color2 = glm::vec4(sin(colorval) * cos(colorval), cos(colorval), sin(colorval), 1.0f);

		Snow::RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.9f, 1.0f });
		Snow::RenderCommand::Clear();

		Snow::Renderer::BeginScene(m_CameraController.GetCamera());


		Snow::Ref<Snow::Shader> ColorPickerShader =	m_ShaderLib.Get("ColorPicker");

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 25; y++)
		{
			for (int x = 0; x < 25; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x*y) % 2 == 0)
				{
					ColorPickerShader->UploadUniformFloat4("u_Color", m_Color1);
				}
				else
				{
					ColorPickerShader->UploadUniformFloat4("u_Color", m_Color2);
				}
				m_Texture->Bind();
				ColorPickerShader->UploadUniformInt("u_Texture", 0);
				Snow::Renderer::Submit(m_SquareVA, ColorPickerShader,transform);
			}
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f));
		Snow::Renderer::Submit(m_SquareVA, m_ShaderLib.Get("Texture"), transform);

		Snow::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Snow::Event& event) override
	{
		m_CameraController.OnEvent(event);
		Snow::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Snow::KeyPressedEvent>(SNOW_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent, 1));
	}
	bool OnKeyPressedEvent(Snow::KeyPressedEvent& event)
	{
		return false;
	}

private:
	Snow::ShaderLibrary m_ShaderLib;
	Snow::OrthographicCameraController m_CameraController;

	Snow::Ref<Snow::VertexArray> m_SquareVA;

	Snow::Ref<Snow::Texture2D> m_Texture;

	float colorval = 0.0f;
	glm::vec4 m_Color1 = glm::vec4(0.0f,0.0f,0.0f,1.0f);
	glm::vec4 m_Color2 = glm::vec4(0.0f,0.0f,0.0f,1.0f);
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
