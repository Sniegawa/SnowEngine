#include <SnowEngine/SnowEngine.h>

class ExampleLayer : public Snow::Layer
{
public:
	ExampleLayer() : Layer("Example") , m_Camera(new Snow::OrthographicCamera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, Snow::CameraParams::OrthographicCameraParams(-1.6f, 1.6f, -0.9f, 0.9f))) 
	{

		

		m_Trianglepos = glm::vec3(0.0f, 0.0f, 0.0f);

		{
			std::string vertexSrc = R"(
		#version 330 core
				
		layout(location = 0) in vec3 a_Position;
		
		uniform mat4 u_ViewProjection;
		uniform mat4 u_ModelMatrix;


		void main()
		{
			gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position,1.0);
		}

		)";

			std::string fragmentSrc = R"(
			#version 330 core
		
			uniform vec4 u_Color;			

			out vec4 color;

			void main()
			{
				color = u_Color;
			}
		

		)";

			m_Shader.reset(Snow::Shader::Create(vertexSrc, fragmentSrc));
		}

		{
			std::string vertexSrc = R"(
		#version 330 core
				
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_TexCoord;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_ModelMatrix;

		out vec2 v_TexCoord;

		void main()
		{
			gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position,1.0);
			v_TexCoord = a_TexCoord;
		}

		)";

			std::string fragmentSrc = R"(
			#version 330 core
				
			uniform sampler2D u_Texture;
			in vec2 v_TexCoord;

			out vec4 color;

			void main()
			{
				color = texture(u_Texture,v_TexCoord);
			}
		

		)";

			m_TextureShader.reset(Snow::Shader::Create(vertexSrc, fragmentSrc));
		}

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


			if (Snow::Input::IsKeyPressed(Snow::Key::A))
			{
				m_Trianglepos += glm::vec3(-1.0f, 0.0f, 0.0f) * dt;
			}
			if (Snow::Input::IsKeyPressed(Snow::Key::D))
			{
				m_Trianglepos += glm::vec3(1.0f, 0.0f, 0.0f) * dt;
			}
			if (Snow::Input::IsKeyPressed(Snow::Key::W))
			{
				m_Trianglepos += glm::vec3(0.0f, 1.0f, 0.0f) * dt;
			}
			if (Snow::Input::IsKeyPressed(Snow::Key::S))
			{
				m_Trianglepos += glm::vec3(0.0f, -1.0f, 0.0f) * dt;
			}

		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Trianglepos);

		Snow::RenderCommand::SetClearColor({ 0.4f, 0.4f, 0.9f, 1.0f });
		Snow::RenderCommand::Clear();

		Snow::Renderer::BeginScene(m_Camera);



		m_Shader->Bind();

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 25; y++)
		{
			for (int x = 0; x < 25; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x*y) % 2 == 0)
				{
					m_Shader->UploadUniformFloat4("u_Color", m_Color1);
				}
				else
				{
					m_Shader->UploadUniformFloat4("u_Color", m_Color2);
				}
				m_Texture->Bind();
				m_Shader->UploadUniformInt("u_Texture", 0);
				Snow::Renderer::Submit(m_SquareVA, m_Shader,transform);
			}
		}

		Snow::Renderer::Submit(m_SquareVA, m_TextureShader, transform);

		Snow::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Color Picker");
		ImGui::ColorPicker4("Odd",glm::value_ptr(m_Color1));
		ImGui::ColorPicker4("Even",glm::value_ptr(m_Color2));
		ImGui::End();
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
	Snow::Ref<Snow::VertexArray> m_SquareVA;

	Snow::Ref<Snow::Shader> m_Shader;
	Snow::Ref<Snow::Shader> m_TextureShader;

	Snow::Ref<Snow::Camera> m_Camera;

	Snow::Ref<Snow::Texture2D> m_Texture;

	glm::vec3 m_Trianglepos;

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
