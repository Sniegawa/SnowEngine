#include "ImGuiLayer.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

#ifdef SNOW_GLFW

#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"

#elif SNOW_SDL

SNOW_CORE_ERROR("No sdl support yet");

#endif


#include "SnowEngine/Engine/Core/Application.h"
namespace Snow
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		Application& app = Application::Get();

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		float FontSize = 17.0f;
		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Bold.ttf", FontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", FontSize);

		SetTheme(ImGuiTheme::DeepIce);

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
#ifdef SNOW_GLFW
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();

#ifdef  SNOW_GLFW
		ImGui_ImplGlfw_NewFrame();
#endif //  SNOW_GLFW

		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef SNOW_GLFW
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
#endif //  SNOW_GLFW

	}

	void ImGuiLayer::SetTheme(ImGuiTheme theme)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		switch (theme)
		{
		case Snow::DeepIce:
			ImGui::StyleColorsDark();
			// Backgrounds
			colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.07f, 0.15f, 1.0f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.08f, 0.18f, 1.0f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.10f, 0.20f, 1.0f);
			colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.02f, 0.05f, 0.12f, 1.0f);

			// Menu bar + title bars
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.06f, 0.10f, 0.20f, 1.0f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.09f, 0.18f, 1.0f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.18f, 0.32f, 1.0f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.03f, 0.05f, 0.10f, 1.0f);

			// Headers / docking / tabs
			colors[ImGuiCol_Header] = ImVec4(0.15f, 0.35f, 0.70f, 1.0f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.45f, 0.85f, 1.0f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.40f, 0.80f, 1.0f);

			colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.25f, 0.55f, 1.0f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.40f, 0.75f, 1.0f);
			colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.35f, 0.70f, 1.0f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.20f, 0.40f, 1.0f);

			// Buttons
			colors[ImGuiCol_Button] = ImVec4(0.18f, 0.45f, 0.85f, 1.0f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.55f, 0.95f, 1.0f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.40f, 0.75f, 1.0f);

			// Frames (inputs, sliders)
			colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.20f, 0.40f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.30f, 0.55f, 1.0f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.40f, 0.70f, 1.0f);

			// Text
			colors[ImGuiCol_Text] = ImVec4(0.85f, 0.95f, 1.0f, 1.0f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.45f, 0.60f, 0.75f, 1.0f);

			// Layout / spacing
			style.FrameRounding = 4.0f;
			style.GrabRounding = 4.0f;
			style.WindowRounding = 6.0f;
			style.ScrollbarRounding = 6.0f;
			style.TabRounding = 4.0f;

			style.WindowPadding = ImVec2(8, 6);
			style.FramePadding = ImVec2(6, 4);
			style.ItemSpacing = ImVec2(8, 6);
			style.ItemInnerSpacing = ImVec2(6, 4);
			style.ScrollbarSize = 14.0f;

			break;
		case Snow::WhiteSnow:
			ImGui::StyleColorsLight();
			// Backgrounds (soft gray-white)
			colors[ImGuiCol_WindowBg] = ImVec4(0.88f, 0.89f, 0.91f, 1.0f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.91f, 0.93f, 1.0f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.92f, 0.93f, 0.95f, 1.0f);
			colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.8f, 0.81f, 0.82f, 1.0f);

			// Menu bar + title bars
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.85f, 0.86f, 0.88f, 1.0f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.86f, 0.88f, 0.90f, 1.0f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.78f, 0.82f, 0.87f, 1.0f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.83f, 0.85f, 0.87f, 1.0f);

			// Headers / docking / tabs
			colors[ImGuiCol_Header] = ImVec4(0.65f, 0.75f, 0.85f, 1.0f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.58f, 0.70f, 0.88f, 1.0f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.65f, 0.83f, 1.0f);

			colors[ImGuiCol_Tab] = ImVec4(0.82f, 0.87f, 0.92f, 1.0f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.72f, 0.80f, 0.90f, 1.0f);
			colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.78f, 0.88f, 1.0f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.85f, 0.90f, 0.94f, 1.0f);

			// Buttons
			colors[ImGuiCol_Button] = ImVec4(0.68f, 0.78f, 0.88f, 1.0f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.58f, 0.70f, 0.85f, 1.0f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.50f, 0.65f, 0.80f, 1.0f);

			// Frames (inputs, sliders)
			colors[ImGuiCol_FrameBg] = ImVec4(0.82f, 0.87f, 0.92f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.72f, 0.80f, 0.88f, 1.0f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.68f, 0.78f, 0.85f, 1.0f);

			// Text
			colors[ImGuiCol_Text] = ImVec4(0.12f, 0.15f, 0.18f, 1.0f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.48f, 0.52f, 1.0f);

			// Layout / spacing
			style.FrameRounding = 4.0f;
			style.GrabRounding = 4.0f;
			style.WindowRounding = 6.0f;
			style.ScrollbarRounding = 6.0f;
			style.TabRounding = 4.0f;

			style.WindowPadding = ImVec2(8, 6);
			style.FramePadding = ImVec2(6, 4);
			style.ItemSpacing = ImVec2(8, 6);
			style.ItemInnerSpacing = ImVec2(6, 4);
			style.ScrollbarSize = 14.0f;
			break;
		default:
			break;
		}
	}
};