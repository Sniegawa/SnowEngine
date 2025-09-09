#include "EditorLayer.h"

namespace Snow
{

	class CameraController : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts)
		{
			glm::mat4& transform = GetComponent<TransformComponent>().Transform;

			glm::vec2 MoveVector = glm::vec2(0.0f);
			float RotationAmount = 0.0f;

			if (Input::IsKeyPressed(Key::A))
			{
				MoveVector.x -= m_MoveSpeed * ts;
			}
			if (Input::IsKeyPressed(Key::D))
			{
				MoveVector.x += m_MoveSpeed * ts;
			}
			if (Input::IsKeyPressed(Key::W))
			{
				MoveVector.y += m_MoveSpeed * ts;
			}
			if (Input::IsKeyPressed(Key::S))
			{
				MoveVector.y -= m_MoveSpeed * ts;
			}
			
			if (Input::IsKeyPressed(Key::Q))
				RotationAmount += m_RotationSpeed * ts;
			if (Input::IsKeyPressed(Key::E))
				RotationAmount -= m_RotationSpeed * ts;

			transform = glm::rotate(transform, RotationAmount, glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::translate(transform, glm::vec3(MoveVector.x, MoveVector.y, 0.0f));
		}
	private:
		float m_MoveSpeed = 2.0f;
		float m_RotationSpeed = 5.0f;
	};


	EditorLayer::EditorLayer()
		:	Layer("Snowball"),
			m_CameraController(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, (1280.0f / 720.0f), true)
	{
	}

	EditorLayer::~EditorLayer()
	{

	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification specs;
		specs.Width = Application::Get().GetWindow().GetWidth();
		specs.Height = Application::Get().GetWindow().GetHeight();

		m_Framebuffer = Framebuffer::Create(specs);

		m_ActiveScene = CreateRef<Scene>();

		m_Entities.resize(4);

		for (size_t i = 0; i < m_Entities.size(); ++i)
		{
			m_Entities[i] = m_ActiveScene->CreateEntity("Square " + std::to_string(i));
			m_Entities[i].AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		
		m_CameraEntity = m_ActiveScene->CreateEntity("CameraEntity");
		auto& camComponent = m_CameraEntity.AddComponent<CameraComponent>();
		camComponent.Primary = true;
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if(m_IsViewportFocused)	m_CameraController.OnUpdate(ts);

		m_Framebuffer->Bind();
		Renderer2D::ResetStats();

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.7f, 1.0f });
		RenderCommand::Clear();

		//Snow::Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ActiveScene->OnUpdate(ts);

		

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		//Todo : Strip out unnecesary code from this imgui demo
		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Snowball", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("Snowball");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Snow::Application::Get().Close();
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Editor"))
			{
				if (ImGui::BeginMenu("Themes"))
				{
					static int currentTheme = 0; // 0 = Dark, 1 = Light

					if (ImGui::MenuItem("Dark", NULL, currentTheme == 0))
					{
						currentTheme = 0;
						ImGuiLayer::SetTheme(ImGuiTheme::DeepIce);
					}

					if (ImGui::MenuItem("Light", NULL, currentTheme == 1))
					{
						currentTheme = 1;
						ImGuiLayer::SetTheme(ImGuiTheme::WhiteSnow);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		ImGui::Begin("Viewport");

		m_IsViewportFocused = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();
		glm::ivec2 viewportSize = { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y };

		if (m_ViewportSize != viewportSize && (viewportSize.x > 0 && viewportSize.y > 0))
		{
			m_ViewportSize = viewportSize;
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachementRendererID();
		ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });


		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Properties");
		for (size_t i = 0; i < m_Entities.size(); ++i)
		{
			ImGui::Text(m_Entities[i].GetComponent<TagComponent>().Tag.c_str());
			if (m_Entities[i].HasComponent<SpriteRendererComponent>())
			{
				ImGui::PushID(i);
				ImGui::Text("Transform");
				ImGui::DragFloat3("Position", glm::value_ptr(m_Entities[i].GetComponent<TransformComponent>().Transform[3]), 0.01f);
				ImGui::Text("SpriteRenderer");
				ImGui::DragFloat4("", &m_Entities[i].GetComponent<SpriteRendererComponent>().Color[0], 0.025f, 0.0f, 1.0f);
				ImGui::PopID();
			}
			ImGui::Spacing();
			ImGui::Spacing();
		}
		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]),0.125f);
		ImGui::Checkbox("Camera Fixed Aspectratio", &m_CameraEntity.GetComponent<CameraComponent>().FixedAspectRatio);
		{
			auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			ImGui::DragFloat("Camera OrthoSize", &orthoSize, 0.125f, 0.25f, 100.0f);
			camera.SetOrthographicSize(orthoSize);

		}ImGui::End();

		auto stats = Snow::Renderer2D::GetStats();
		std::stringstream Drawcals;
		Drawcals << "Drawcalls : " << stats.DrawCalls << std::endl;
		std::stringstream Quads;
		Drawcals << "Quads : " << stats.QuadCount << std::endl;
		std::stringstream Vertices;
		Drawcals << "Vertices : " << stats.GetTotalVertexCount();
		ImGui::Begin("Renderer Stats");
		ImGui::Text(Drawcals.str().c_str());
		ImGui::Text(Quads.str().c_str());
		ImGui::Text(Vertices.str().c_str());
		ImGui::End();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_IsViewportHovered)	m_CameraController.OnEvent(e);
	}

};