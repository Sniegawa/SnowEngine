#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include <entt.hpp>

namespace Snow
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (m_Context)
		{

			for (auto entityID : m_Context->m_Registry.storage<entt::entity>())
			{
				Entity entity{ entityID,m_Context.get() };
				DrawEntityNode(entity);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			//Display child entity
			ImGui::TreePop();
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
			
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags,name.c_str());
			ImGui::PopStyleVar();
			if(typeid(T) != typeid(TransformComponent))
			{
				if (ImGui::Button("+", ImVec2{ 16,16 }))
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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float speed = 0.1f, float resetValue = 0.0f, float columntWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columntWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3,ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		
		float lineHeight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f,0.1f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f,0.1f,0.1f,1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.7f,0.1f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.7f,0.1f,1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.1f,0.7f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.1f,0.7f,1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();

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
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
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
				float Size = glm::degrees(camera.GetPerspectiveFOV());
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

		DrawComponent<MusicEmitterComponent>("Music Emitter", entity, [](MusicEmitterComponent& component)
		{
			ImGui::DragFloat("Volume", &component.Config.volume, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Pitch", &component.Config.pitch, 0.05f, 0.0f, 10.0f);
			ImGui::DragFloat("Close Range", &component.Config.minDistance, 0.05f, 0.0f);
			ImGui::DragFloat("Far Range", &component.Config.maxDistance, 0.05f, 0.0f);
			//Add some support for toggling the Looping during runtime of music
			ImGui::Checkbox("Will loop", &component.Config.looping);
			
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

		DrawComponent<SoundEmitterComponent>("Sound Emitter", entity, [](SoundEmitterComponent& component)
		{
				ImGui::DragFloat("Volume", &component.Config.volume, 0.05f, 0.0f, 1.0f);
				ImGui::DragFloat("Pitch", &component.Config.pitch, 0.05f, 0.0f, 10.0f);
				ImGui::DragFloat("Close Range", &component.Config.minDistance, 0.05f, 0.0f);
				ImGui::DragFloat("Far Range", &component.Config.maxDistance, 0.05f, 0.0f);

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