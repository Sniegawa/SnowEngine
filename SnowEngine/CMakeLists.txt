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

add_library(SnowEngine SHARED "Engine/Core/Core.cpp" "Engine/Core/Core.h" ${IMGUI_SOURCES}    "Engine/SnowEngineAPI.h"  "SnowEngine.h" "Engine/Core/Log.h" "Engine/Core/Log.cpp")

#Export as dll if on Windows
if (WIN32)
    target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS SNOW_BUILD_DLL)
else()
    target_compile_definitions(SnowEngine PUBLIC SNOW_LINUX)
endif()

#Spdlog error fix
if (MSVC)
    target_compile_options(SnowEngine PRIVATE "/utf-8")
endif()

target_include_directories(SnowEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_include_directories(SnowEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
    ${CMAKE_SOURCE_DIR}/Libs/glew/include
    ${CMAKE_SOURCE_DIR}/Libs/glm
	${CMAKE_SOURCE_DIR}/Libs/spdlog/include
	${IMGUI_DIR}
)

target_link_libraries (SnowEngine PUBLIC
		glfw
		libglew_static
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