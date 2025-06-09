cmake_minimum_required(VERSION 3.8)

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

add_library(SnowEngine SHARED ${IMGUI_SOURCES}
	"Engine/Core/Core.h" 
	"Engine/Core/Core.cpp" 
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
	"Engine/Core/Platform/Windows/WindowsWindow.h" 
	"Engine/Core/Platform/Windows/WindowsWindow.cpp" 
	"Engine/Core/Layers/Layer.h" 
	"Engine/Core/Layers/LayerStack.h" 
	"Engine/Core/Layers/LayerStack.cpp" 
	"EntryPoint.h")

#Export as dll if on Windows
if (WIN32)
    target_compile_definitions(SnowEngine PRIVATE SNOW_BUILD_DLL)
	target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS)
else()
    target_compile_definitions(SnowEngine PUBLIC SNOW_LINUX)
endif()

target_compile_definitions(SnowEngine PUBLIC GLFW_INCLUDE_NONE)

#Spdlog error fix
if (MSVC)
    target_compile_options(SnowEngine PRIVATE "/utf-8")
	target_compile_options(SnowEngine PRIVATE /GR)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
       CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(SnowEngine PRIVATE -frtti)
endif()


target_include_directories(SnowEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_include_directories(SnowEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
	${CMAKE_SOURCE_DIR}/Libs/GLAD/include
    ${CMAKE_SOURCE_DIR}/Libs/glm
	${CMAKE_SOURCE_DIR}/Libs/spdlog/include
	${IMGUI_DIR}
)

target_link_libraries (SnowEngine PUBLIC
		glfw
		GLAD
		spdlog::spdlog
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)


add_custom_command(TARGET SnowEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_BINARY_DIR}/SnowEngine.dll"
        "${CMAKE_BINARY_DIR}/SnowEngineApp/SnowEngine.dll"
)

message(CMAKE_BINARY_DIR ="${CMAKE_BINARY_DIR}")