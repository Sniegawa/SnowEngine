cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SnowEngine")


#include_directories("imgui/")
#file(
#	GLOB 3rdPartyImGui
#		"imgui/imgui.cpp"
#		"imgui/imgui_draw.cpp"
#		"imgui/imgui_demo.cpp"
#		"imgui/imgui_tables.cpp"
#		"imgui/imgui_widgets.cpp"
#		"imgui/*.h"
#)
#source_group(ImGui FILES ${3rdPartyImGui})



add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory("SnowEngine")
# IMGUI AND STB


target_include_directories(SnowEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories(SnowEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
target_include_directories(SnowEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glm)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SnowEngine PROPERTY CXX_STANDARD 20)
endif()
