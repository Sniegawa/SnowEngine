cmake_minimum_required(VERSION 3.10)

project("SnowEngine")

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui)

set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_impl_glfw.cpp
	${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

add_library(SnowEngine STATIC
	${IMGUI_SOURCES} 
	"Engine/Core/Application.h" 
	"Engine/Core/Application.cpp" 
	"Engine/SnowEngineAPI.h"  
	"SnowEngine.h" 
	"Engine/Core/Logging/Log.h" 
	"Engine/Core/Logging/Log.cpp" 
	"Engine/Core/Events/Event.h" 
	"Engine/Core/Events/KeyEvent.h" 
	"Engine/Core/KeyCode.h" 
	"Engine/Core/Events/MouseEvent.h" 
	"Engine/Core/MouseCode.h" 
	"Engine/Core/Events/ApplicationEvent.h"
	"Engine/Core/IWindow.h" 
	"Engine/Platform/Windows/WindowsWindow.h" 
	"Engine/Platform/Windows/WindowsWindow.cpp" 
	"Engine/Core/Layers/Layer.h" 
	"Engine/Core/Layers/LayerStack.h" 
	"Engine/Core/Layers/LayerStack.cpp" 
	"Engine/Core/ImGui/ImGuiLayer.h" 
	"Engine/Core/ImGui/ImGuiLayer.cpp"
	"Engine/Core/Input.h" 
	"Engine/Platform/Windows/WindowsInput.h" 
	"Engine/Platform/Windows/WindowsInput.cpp" 
	"EntryPoint.h" 
	"Engine/Core/Renderer/GraphicsContext.h" 
	"Engine/Platform/OpenGL/OpenGLContext.h" 
	"Engine/Platform/OpenGL/OpenGLContext.cpp" 
	"Engine/Core/Renderer/Shader.h" 
	"Engine/Core/Renderer/Shader.cpp" 
	"Engine/Core/Renderer/Buffer.h" 
	"Engine/Core/Renderer/Buffer.cpp" 
	"Engine/Platform/OpenGL/OpenGLBuffer.h" 
	"Engine/Platform/OpenGL/OpenGLBuffer.cpp" 
	"Engine/Core/Renderer/Renderer.h" 
	"Engine/Core/Renderer/Renderer.cpp" 
	"Engine/Core/Renderer/VertexArray.h" 
	"Engine/Core/Renderer/VertexArray.cpp" 
	"Engine/Platform/OpenGL/OpenGLVertexArray.cpp" 
	"Engine/Platform/OpenGL/OpenGLVertexArray.h" 
	"Engine/Core/Renderer/Camera/Camera.h" 
	"Engine/Core/Renderer/RendererAPI.h" 
	"Engine/Core/Renderer/RendererAPI.cpp" 
	"Engine/Core/Renderer/RenderCommand.h" 
	"Engine/Core/Renderer/RenderCommand.cpp" 
	"Engine/Platform/OpenGL/OpenGLRendererAPI.h" 
	"Engine/Platform/OpenGL/OpenGLRendererAPI.cpp" 
	"Engine/Core/Renderer/Camera/OrthographicCamera.h" 
	"Engine/Core/Renderer/Camera/OrthographicCamera.cpp" 
	"Engine/Core/Timestep.h" 
	"Engine/Platform/OpenGL/OpenGLShader.h" 
	"Engine/Platform/OpenGL/OpenGLShader.cpp" 
	"Engine/Core/Renderer/Texture.h" 
	"Engine/Core/Renderer/Texture.cpp"   
	"Engine/Platform/OpenGL/OpenGLTexture.h" 
	"Engine/Platform/OpenGL/OpenGLTexture.cpp" 
	"ThirdParty/stb/stb_image.h"
	"ThirdParty/stb/stb_image.cpp" 
	"Engine/Core/Renderer/Camera/CameraController.h" 
	"Engine/Core/Renderer/Camera/OrthographicCameraController.h" 
	"Engine/Core/Renderer/Camera/OrthographicCameraController.cpp" "Engine/Core/Renderer/Renderer2D.cpp" "Engine/Core/Renderer/Renderer2D.h" "Engine/Core/Renderer/Spritesheet.h" "Engine/Core/Renderer/Spritesheet.cpp")

#Export as dll if on Windows
if (WIN32)
    #target_compile_definitions(SnowEngine PRIVATE SNOW_BUILD_DLL)
	target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS)
else()
    target_compile_definitions(SnowEngine PUBLIC SNOW_LINUX)
endif()

target_compile_definitions(SnowEngine PUBLIC SNOW_GLFW)
target_compile_definitions(SnowEngine PUBLIC GLFW_INCLUDE_NONE)


#Spdlog error fix
if (MSVC)
    target_compile_options(SnowEngine PRIVATE "/utf-8")
	target_compile_options(SnowEngine PRIVATE /GR)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(SnowEngine PRIVATE -frtti)
endif()

target_compile_options(SnowEngine PRIVATE
    $<$<CONFIG:Debug>:/Zi /Od /MDd> # MSVC debug flags for Debug config
)


target_include_directories(SnowEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/Engine
)


target_include_directories(SnowEngine PUBLIC
	${CMAKE_SOURCE_DIR}/Libs/GLAD/include
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
    ${CMAKE_SOURCE_DIR}/Libs/glm
	${CMAKE_SOURCE_DIR}/Libs/spdlog/include
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb
	${IMGUI_DIR}

)

target_link_libraries (SnowEngine PUBLIC
		GLAD
		glfw
		spdlog::spdlog_header_only
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(SnowEngine PRIVATE SNOW_DEBUG)
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)


add_custom_command(TARGET SnowEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_BINARY_DIR}/SnowEngine.lib"
        "${CMAKE_BINARY_DIR}/SnowEngineApp/SnowEngine.lib"
)