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

add_library(SnowEngine SHARED "Engine/Core/Core.cpp" "Engine/Core/Core.h" ${IMGUI_SOURCES}    "Engine/SnowEngineAPI.h" )

#Export as dll if on Windows
if (WIN32)
    target_compile_definitions(SnowEngine PUBLIC SNOW_WINDOWS SNOW_BUILD_DLL)
else()
    target_compile_definitions(SnowEngine PUBLIC SNOW_LINUX)
endif()


target_include_directories(SnowEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_include_directories(SnowEngine PUBLIC
    ${CMAKE_SOURCE_DIR}/Libs/glfw/include
    ${CMAKE_SOURCE_DIR}/Libs/glew/include
    ${CMAKE_SOURCE_DIR}/Libs/glm
	${IMGUI_DIR}
)

target_link_directories(SnowEngine PUBLIC
	${CMAKE_SOURCE_DIR}/Libs/glfw/lib
	${CMAKE_SOURCE_DIR}/Libs/glew/lib
)

target_link_libraries (SnowEngine
		glfw
		libglew_static
)