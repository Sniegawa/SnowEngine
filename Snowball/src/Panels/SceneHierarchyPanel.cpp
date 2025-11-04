#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>

#include <filesystem>

namespace Snow
{

	extern const std::filesystem::path g_AssetsPath;


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (m_Context)
		{
			Entity EntityToDestroy = {};
			auto view = m_Context->m_Registry.view<TagComponent>();
			std::vector<entt::entity> entities(view.begin(), view.end());
			std::sort(entities.begin(),entities.end());
			for (auto entityID : entities)
			{
				Entity entity{ entityID,m_Context.get() };
				DrawEntityNode(entity,&EntityToDestroy);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			if (EntityToDestroy)
			{
				m_Context->DestroyEntity(EntityToDestroy);
				if (m_SelectionContext == EntityToDestroy)
					m_SelectionContext = {};
			}
			
			//Right click blank space
			if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create new entity"))
					m_Context->CreateEntity();

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			ImGui::Separator();
			float windowWidth = ImGui::GetWindowSize().x;
			float buttonWidth = ImGui::CalcTextSize("Add Component").x + ImGui::GetStyle().FramePadding.x * 2.0f;

			ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

			float padding = 20.0f; // extra horizontal padding
			ImVec2 textSize = ImGui::CalcTextSize("Add Component");
			ImVec2 buttonSize = ImVec2(
				textSize.x + ImGui::GetStyle().FramePadding.x * 2.0f + padding,
				textSize.y + ImGui::GetStyle().FramePadding.y * 2.0f + 8.0f // taller
			);

			ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[SNOW_FONT_BOLD]);
			if (ImGui::Button("Add Component",buttonSize))
				ImGui::OpenPopup("AddComponent");

			ImGui::PopFont();

			if (ImGui::BeginPopup("AddComponent"))
			{
				DisplayAddComponentEntry<CameraComponent>("Camera");
				DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
				DisplayAddComponentEntry<AudioEmitterComponent>("Audio Emitter");
				DisplayAddComponentEntry<AudioListenerComponent>("Audio Listener");
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity,Entity* entityToDestroy)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			//Display child entity
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			*entityToDestroy = entity;

		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags,name.c_str());
			ImGui::PopStyleVar();

			if(typeid(T) != typeid(TransformComponent))
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight}))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
			}
			

			bool removeComponent = false;
			if(ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("RemoveComponent"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float speed = 0.1f, float resetValue = 0.0f, float columntWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[SNOW_FONT_BOLD]; // Bold

		ImGui::PushID(label.c_str());


		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columntWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3,3 });
		
		float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		/// X BUTTON

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f,0.1f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f,0.1f,0.1f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y BUTTON

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.7f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.7f,0.1f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z BUTTON

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.1f,0.7f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.1f,0.7f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		ImGui::PopStyleVar();
		
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
    #ifdef SNOW_LINUX
      strcpy(buffer,tag.c_str());
    #else
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
    #endif	
    if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
		{
			DrawVec3Control("Position", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation,5.0f);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& camera = component.Camera;
			
			const char* porjectionTypeStrings[] = { "Perspective","Orthographic" };
			int currentIndex = static_cast<int>(camera.m_ProjectionType);
			int count = sizeof(porjectionTypeStrings) / sizeof(porjectionTypeStrings[0]);
			if (ImGui::BeginCombo("Projection", porjectionTypeStrings[currentIndex]))
			{
				for (int i = 0; i < count; ++i)
				{
					bool isSelected = (currentIndex == i);
					if (ImGui::Selectable(porjectionTypeStrings[i], isSelected))
						currentIndex = i;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(currentIndex));
				ImGui::EndCombo();
			}

			if (camera.m_ProjectionType == SceneCamera::ProjectionType::Orthographic)
			{
				float Size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &Size, 0.1f, 0.0f))
					camera.SetOrthographicSize(Size);
				float Near = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &Near, 1.0f, 0.0f))
					camera.SetOrthographicNearClip(Near);
				float Far = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &Far, 1.0f, 0.0f))
					camera.SetOrthographicFarClip(Far);

				ImGui::Checkbox("Fixed aspect ratio", &component.FixedAspectRatio);
			}
			
			if (camera.m_ProjectionType == SceneCamera::ProjectionType::Perspective)
			{
				float Size = camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("Fov", &Size, 1.0f, 1.0f,359.0f))
					camera.SetPerspectiveFOV(Size);
				float Near = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &Near, 1.0f, 0.0f))
					camera.SetPerspectiveNearClip(Near);
				float Far = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &Far, 1.0f, 0.0f))
					camera.SetPerspectiveFarClip(Far);
			}

			ImGui::Checkbox("Primary", &component.Primary);
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Text("Texture");
			ImGui::SameLine();
			//Texture
			if (component.SpriteTexture)
				ImGui::ImageButton("TextureControl", (ImTextureID)component.SpriteTexture->GetRendererID(), { 100,100 }, { 0,1 }, { 1,0 });
			else
				ImGui::Button("Empty Texture", ImVec2(100.0f, 100.0f));

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
				if (payload)
				{
					auto path = std::filesystem::path((const char *)payload->Data);
					auto file_path = std::filesystem::path(g_AssetsPath) / path;
					if(path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg")
						component.SpriteTexture = Texture2D::Create(file_path.string());
						
				}
				ImGui::EndDragDropTarget();
			}

		});

		DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](AudioListenerComponent& component)
		{
			int listID = component.ListenerID;
			ImGui::Text("Only support for listenerID = 0.\nThis control does nothing!");
			if (ImGui::DragInt("ListenerID", &listID,1.0f,0,4))
			{
				//component.ListenerID = listID;
			}
			ImGui::DragFloat("Volume", &component.masterVolume, 0.05f, 0.0f, 1.0f);
		});

		DrawComponent<AudioEmitterComponent>("Music Emitter", entity, [](AudioEmitterComponent& component)
		{
			ImGui::DragFloat("Volume", &component.Config.volume, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Pitch", &component.Config.pitch, 0.05f, 0.0f, 10.0f);
			ImGui::DragFloat("Close Range", &component.Config.minDistance, 0.05f, 0.0f);
			ImGui::DragFloat("Far Range", &component.Config.maxDistance, 0.05f, 0.0f);
			//Add some support for toggling the Looping during runtime of music
			ImGui::Checkbox("Will loop", &component.Config.loop);
			
			AttenuationModel& model = component.Config.attenuation;
			const char* AttenuationModelStrings[] = { "Linear", "Inverse", "Exponential" };
			int currentIndex = static_cast<int>(model);
			int count = sizeof(AttenuationModelStrings) / sizeof(AttenuationModelStrings[0]);
			if (ImGui::BeginCombo("Attenuation Model", AttenuationModelStrings[currentIndex]))
			{
				for (int i = 0; i < count; ++i)
				{
					bool isSelected = (currentIndex == i);
					if (ImGui::Selectable(AttenuationModelStrings[i], isSelected))
						currentIndex = i;

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			model = static_cast<AttenuationModel>(currentIndex);
		});
	}

};
