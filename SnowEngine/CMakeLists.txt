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

set(SNOW_SOURCES
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
	"Engine/Core/Layers/Layer.h" 
	"Engine/Core/Layers/LayerStack.h" 
	"Engine/Core/Layers/LayerStack.cpp" 
	"Engine/Core/ImGui/ImGuiLayer.h" 
	"Engine/Core/ImGui/ImGuiLayer.cpp"
	"Engine/Core/Input.h" 
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
	"Engine/Core/Renderer/Camera/CameraController.h" 
	"Engine/Core/Renderer/Camera/OrthographicCameraController.h" 
	"Engine/Core/Renderer/Camera/OrthographicCameraController.cpp" 
	"Engine/Core/Renderer/Renderer2D.cpp" 
	"Engine/Core/Renderer/Renderer2D.h" 
	"Engine/Core/Renderer/Spritesheet.h" 
	"Engine/Core/Renderer/Spritesheet.cpp" 
	"Engine/Core/Audio/AudioSystem.h" 
	"Engine/Core/Audio/AudioSystem.cpp" 
	"Engine/Core/Audio/SoundInstance.h" 
	"Engine/Core/Audio/SoundInstance.cpp" 
	"Engine/Core/Audio/MiniAduioBackend.cpp"
	"Engine/Core/Audio/MusicInstance.h"
	"Engine/Core/Audio/MusicInstance.cpp"
	"Engine/Core/Audio/AudioAssets.h"
	"Engine/Core/Renderer/Framebuffer.h"
	"Engine/Core/Renderer/Framebuffer.cpp"
	"Engine/Platform/OpenGL/OpenGLFramebuffer.h" 
	"Engine/Platform/OpenGL/OpenGLFramebuffer.cpp" "Engine/Core/Scene/Scene.h" "Engine/Core/Scene/Scene.cpp" "Engine/Core/Scene/Components.h")

set(THIRD_PARTY_SOURCES
	"ThirdParty/miniaudio/miniaudio.h" 
	"ThirdParty/entt/entt.hpp"
	"ThirdParty/stb/stb_image.h"
	"ThirdParty/stb/stb_image.cpp" 
)

if(WIN32)
	set(PLATFORM_SOURCES
		"Engine/Platform/Windows/WindowsInput.cpp" 
		"Engine/Platform/Windows/WindowsWindow.h" 
		"Engine/Platform/Windows/WindowsWindow.cpp" 
	)
elseif(UNIX)
	set(PLATFORM_SOURCES
		 "Engine/Platform/Linux/LinuxInput.cpp" 
		 "Engine/Platform/Linux/LinuxWindow.h" 
		 "Engine/Platform/Linux/LinuxWindow.cpp"
	)
endif()


add_library(SnowEngine STATIC
	${IMGUI_SOURCES} 
	${THIRD_PARTY_SOURCES}
	${SNOW_SOURCES}
	${PLATFORM_SOURCES}
)

#Export as dll if on Windows
if (WIN32)
    #target_compile_definitions(SnowEngine PRIVATE SNOW_BUILD_DLL)
	target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS)
elseif(UNIX)
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
	
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/entt
)

target_include_directories(SnowEngine PUBLIC
	${IMGUI_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/miniaudio
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

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${SNOW_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty" FILES ${THIRD_PARTY_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" FILES ${IMGUI_SOURCES})

if(MSVC)
add_custom_command(TARGET SnowEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_BINARY_DIR}/SnowEngine.lib"
        "${CMAKE_BINARY_DIR}/SnowEngineApp/SnowEngine.lib"
)
endif()
