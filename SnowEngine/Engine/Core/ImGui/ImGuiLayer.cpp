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
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		
#ifdef SNOW_GLFW
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
#endif
		
		ImGui_ImplOpenGL3_Init("#version 410");
	}


	void ImGuiLayer::OnUpdate()
	{
		static ImVec4 color = ImVec4(0.1f,0.1f,0.7f,1.0f);
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
#ifdef SNOW_GLFW
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;
#endif

		ImGui::Begin("Color picker");

		ImGui::ColorPicker3("Background color", (float*)&color);
		app.GetWindow().SetClearColor(glm::vec4(color.x, color.y, color.z, color.w));
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	
	void ImGuiLayer::OnEvent(Event& event)
	{
		//EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<MouseButtonPressedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent,1));
		//dispatcher.Dispatch<MouseButtonReleasedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent,1));
		//dispatcher.Dispatch<MouseMovedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent,1));
		//dispatcher.Dispatch<MouseScrolledEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent,1));
		//dispatcher.Dispatch<KeyPressedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent,1));
		//dispatcher.Dispatch<KeyReleasedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent,1));
		//dispatcher.Dispatch<KeyTypedEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent,1));
		//dispatcher.Dispatch<WindowResizeEvent>(SNOW_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent,1));

	}
/* Leaving it here for now, works a bit, resize mouse fully working, from keyboard only typing chars work
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.AddMouseButtonEvent(e.GetMouseButton(), true);

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.AddMouseButtonEvent(e.GetMouseButton(), false);

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.AddMousePosEvent(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyShift = io.KeysDown[340] || io.KeysDown[344];
		io.KeyCtrl = io.KeysDown[341] || io.KeysDown[345];
		io.KeyAlt = io.KeysDown[342] || io.KeysDown[346];
		io.KeySuper = io.KeysDown[343] || io.KeysDown[347];

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.KeysDown[e.GetKeyCode()] = false;
		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto c = e.GetKeyCode();
		if (c > 0 && c < 0x10000)
			io.AddInputCharacter((unsigned short)c);
		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		
		return false;
	}*/
};