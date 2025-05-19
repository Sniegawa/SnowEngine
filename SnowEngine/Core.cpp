#include "Core.h"

SnowEngine::Application::Application()
{
	initWindow();
	initOpenGL();
	std::cout << "Application initialized" << std::endl;
}

SnowEngine::Application::~Application()
{
	//cleanup();
	std::cout << "Application destroyed" << std::endl;
}

void SnowEngine::Application::initOpenGL()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return;
	}
	glewExperimental = true;
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);//Anti aliasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glViewport(0, 0, 800, 600);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SnowEngine::Application::initWindow()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "SnowEngine", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
}

int SnowEngine::Application::run()
{
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Hello, ImGui!");
		ImGui::Text("Welcome to SnowEngine!");
		ImGui::End();


		ImGui::Render();


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	return 0;
}