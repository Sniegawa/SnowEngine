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
	"Engine/Platform/Windows/WindowsInput.cpp" "EntryPoint.h")

#Export as dll if on Windows
if (WIN32)
    #target_compile_definitions(SnowEngine PRIVATE SNOW_BUILD_DLL)
	target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS)
else()
    target_compile_definitions(SnowEngine PUBLIC SNOW_LINUX)
endif()

target_compile_definitions(SnowEngine PUBLIC SNOW_GLFW)
target_compile_definitions(SnowEngine PUBLIC GLFW_INCLUDE_NONE)
target_compile_definitions(SnowEngine PUBLIC IMGUI_HAS_DOCK)
target_compile_definitions(SnowEngine PUBLIC IMGUI_HAS_VIEWPORT)
target_compile_definitions(SnowEngine PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)


#Spdlog error fix
if (MSVC)
    target_compile_options(SnowEngine PRIVATE "/utf-8")
	target_compile_options(SnowEngine PRIVATE /GR)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(SnowEngine PRIVATE -frtti)
endif()


target_include_directories(SnowEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_include_directories(SnowEngine PUBLIC
	${CMAKE_SOURCE_DIR}/Libs/GLAD/include
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
    ${CMAKE_SOURCE_DIR}/Libs/glm
	${CMAKE_SOURCE_DIR}/Libs/spdlog/include
	${IMGUI_DIR}
)

target_link_libraries (SnowEngine PUBLIC
		GLAD
		glfw
		spdlog::spdlog
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)


add_custom_command(TARGET SnowEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_BINARY_DIR}/SnowEngine.lib"
        "${CMAKE_BINARY_DIR}/SnowEngineApp/SnowEngine.lib"
)

message(CMAKE_BINARY_DIR ="${CMAKE_BINARY_DIR}")