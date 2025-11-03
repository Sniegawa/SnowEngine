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
	//To be changed when projects are a thing
	extern const std::filesystem::path g_AssetsPath = "Assets";


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
		:	Layer("Snowball")
	{
	}

	EditorLayer::~EditorLayer()
	{

	}

	void EditorLayer::OnAttach()
	{
		AudioSystem::LoadAudio("Coin", "Assets/Audio/pickupCoin.wav");
		AudioSystem::LoadAudio("Music","Assets/Audio/musicTest.mp3",AudioType::Music);

		FramebufferSpecification fbspecs;

		fbspecs.Attachments = { FramebufferTextureFormat::RGBA8 , FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };

		fbspecs.Width = Application::Get().GetWindow().GetWidth();
		fbspecs.Height = Application::Get().GetWindow().GetHeight();

		m_Framebuffer = Framebuffer::Create(fbspecs);

		m_ActiveScene = CreateRef<Scene>("MyExampleScene");

		const float fov = 30.0f;
		const float aspect = 1.778f;
		const float nearClip = 0.1f;
		const float farClip = 1000.0f;
		m_EditorCamera = EditorCamera(fov, aspect, nearClip, farClip);

		m_HierarchyPanel.SetContext(m_ActiveScene);

		Application::Get().GetWindow().SetIcon("Resources/Icons/Snowball-logo.png");
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize((float)m_ViewportSize.x, (float)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_EditorCamera.OnUpdate(ts);
		m_Framebuffer->Bind();
		Renderer2D::ResetStats();

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		//Clear entityID attachment
		int ClearValue = -1;
		m_Framebuffer->ClearAttachment(1, &ClearValue);
		
		//Update scene
		m_ActiveScene->OnUpdateEditor(ts,m_EditorCamera);
		//m_ActiveScene->OnUpdateRuntime(ts);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		//Flip to match opengls texture coord system
		my = viewportSize.y - my;
		
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int id = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = id > -1 ? Entity((entt::entity)id, m_ActiveScene.get()) : Entity();
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		//Todo : Strip out unnecessary code from this imgui demo
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

		auto viewportOffset = ImGui::GetCursorPos();

		m_IsViewportFocused = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_IsViewportFocused && !m_IsViewportHovered);



		glm::ivec2 viewportSize = { (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y };

		if ((m_ViewportSize.x != viewportSize.x || m_ViewportSize.y != viewportSize.y) && (viewportSize.x > 0 && viewportSize.y > 0))
		{
			SNOW_CORE_TRACE("Resizing framebuffer from {0},{1} to {2},{3}", m_ViewportSize.x, m_ViewportSize.y, viewportSize.x, viewportSize.y);
			m_ViewportSize = viewportSize;
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachementRendererID(0);
		ImGui::Image(textureID, ImVec2((float)m_ViewportSize.x, (float)m_ViewportSize.y), ImVec2{ 0.0f,1.0f }, ImVec2{ 1.0f,0.0f });

		if(ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
			if (payload)
			{
				auto path = (const wchar_t*)payload->Data;
				auto file_path = std::filesystem::path(path);
				std::string extesion = file_path.extension().string();
				
				if(extesion == ".snow")
					OpenScene(g_AssetsPath / path);
				else if(extesion == ".png" || extesion == ".jpg" || extesion == ".jpeg")
				{
					if(m_HoveredEntity)
					{
						if(m_HoveredEntity.HasComponent<SpriteRendererComponent>())
						{
							auto& src = m_HoveredEntity.GetComponent<SpriteRendererComponent>();
							src.SpriteTexture = Texture2D::Create((g_AssetsPath / path).string());
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		float tabBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y + tabBarHeight };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y - tabBarHeight };

		//Gizmos
		Entity selectedEntity = m_HierarchyPanel.GetSelectedEntity();

		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
			ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

			ImVec2 viewportPos = { windowPos.x + contentMin.x, windowPos.y + contentMin.y };
			ImVec2 viewportSize = { contentMax.x - contentMin.x, contentMax.y - contentMin.y };

			ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);

			const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
			const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();

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
			std::string HoveredEntityName = "None";
			if (m_HoveredEntity)
			{
				HoveredEntityName = m_HoveredEntity.GetComponent<TagComponent>();
			}

			auto stats = Snow::Renderer2D::GetStats();
			std::stringstream Drawcals;
			Drawcals << "Draw calls : " << stats.DrawCalls << std::endl;
			std::stringstream Quads;
			Drawcals << "Quads : " << stats.QuadCount << std::endl;
			std::stringstream Vertices;
			Drawcals << "Vertices : " << stats.GetTotalVertexCount();
			ImGui::Begin("Renderer Stats");
			ImGui::Text("Hovered entity : %s", HoveredEntityName.c_str());
			ImGui::Text(Drawcals.str().c_str());
			ImGui::Text(Quads.str().c_str());
			ImGui::Text(Vertices.str().c_str());
			ImGui::End();
		}

		m_HierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
 
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SNOW_BIND_EVENT_FN(EditorLayer::OnKeyPressed,1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SNOW_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed, 1));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//Shortcuts
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

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseButton::ButtonLeft)
		{
			if(m_IsViewportHovered && !Input::IsKeyPressed(Key::LeftAlt) && !ImGuizmo::IsOver())
				m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return true;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Snow Scene (*.snow)\0*.snow\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string()); //To change to std::filesystem
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
