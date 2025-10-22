#include "EditorLayer.h"
#include "Core/Audio/AudioSystem.h"
#include "Core/KeyCode.h"
#include "Core/Scene/Components.h"

#include "Core/Scene/SceneSerializer.h"

#include "Utilities/PlatformUtils.h"

#include <ImGuizmo.h>

#include <glm/gtx/matrix_decompose.hpp>

namespace Snow
{

	class CameraController : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts)
		{
			glm::mat4 transform = GetComponent<TransformComponent>().GetTransform();

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
		AudioSystem::LoadSound("Coin", "Assets/Audio/pickupCoin.wav");
		AudioSystem::LoadMusic("Music","Assets/Audio/musicTest.mp3");

		FramebufferSpecification specs;
		specs.Width = Application::Get().GetWindow().GetWidth();
		specs.Height = Application::Get().GetWindow().GetHeight();

		m_Framebuffer = Framebuffer::Create(specs);

		m_ActiveScene = CreateRef<Scene>("MyExampleScene");

		m_Hierarchy.SetContext(m_ActiveScene);

		Application::Get().GetWindow().SetIcon("Assets/EditorImages/Snowball-logo.png");
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if(m_IsViewportFocused)	m_CameraController.OnUpdate(ts);
		m_Framebuffer->Bind();
		Renderer2D::ResetStats();

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

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

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open","Ctrl+0"))
					OpenScene();

				if (ImGui::MenuItem("Save As","Ctrl+Shift+S"))
					SaveSceneAs();

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

			m_CameraController.OnResize((float)m_ViewportSize.x, (float)m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachementRendererID();
		ImGui::Image(textureID, ImVec2((float)m_ViewportSize.x, (float)m_ViewportSize.y), ImVec2{ 0.0f,1.0f }, ImVec2{ 1.0f,0.0f });

		
		//Gizmos
		Entity selectedEntity = m_Hierarchy.GetSelectedEntity();

		if (selectedEntity && m_ActiveScene->GetPrimaryCameraEntity())
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			/*
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			*/

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
			ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

			ImVec2 viewportPos = { windowPos.x + contentMin.x, windowPos.y + contentMin.y };
			ImVec2 viewportSize = { contentMax.x - contentMin.x, contentMax.y - contentMin.y };

			ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

			auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());


			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.1f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 22.5f;

			float snapValues[3] = { snapValue,snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),nullptr,(snap ? snapValues : nullptr));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, scale, skew;
				glm::vec4 perspective;
				glm::quat rotation;
				glm::decompose(transform, scale, rotation, translation, skew, perspective);
				
				tc.Translation = translation;
				glm::vec3 deltaRotation = glm::eulerAngles(rotation) - tc.Rotation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		//Additional stats
		{
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
		}

		m_Hierarchy.OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_IsViewportHovered)	m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SNOW_BIND_EVENT_FN(EditorLayer::OnKeyPressed,1));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//Shortucts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		case Key::N:
			if (control)
			{
				NewScene();
			}
			break;
		case Key::O:
			if (control)
			{
				OpenScene();
			}
			break;
		case Key::Q:
			if(!ImGuizmo::IsUsing()) m_GizmoType = -1;
			break;
		case Key::W:
			if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::TRANSLATE;
			break;
		case Key::E:
			if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::ROTATE;
			break;
		case Key::R:
			if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::SCALE;
			break;
		}

		return true;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_Hierarchy.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Snow Scene (*.snow)\0*.snow\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Hierarchy.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Snow Scene (*.snow)\0*.snow\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer = SceneSerializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
};
