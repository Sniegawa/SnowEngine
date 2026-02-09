cmake_minimum_required(VERSION 3.16)

project(Snowball)

add_executable(Snowball 
    "src/main.cpp" 
    "src/EditorLayer.h" 
    "src/EditorLayer.cpp" 
    "src/Panels/SceneHierarchyPanel.h" 
    "src/Panels/SceneHierarchyPanel.cpp"
    "src/Panels/ContentBrowserPanel.h"
    "src/Panels/ContentBrowserPanel.cpp" 
    "src/Tools/ProjectManager.h" 
    "src/Tools/ProjectManager.cpp")

target_include_directories(Snowball PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Snowball
	SnowEngine
)

if(MSVC)
  target_compile_options(Snowball PRIVATE $<$<CONFIG:Debug>:/Zi /Od /MDd>)
endif()


   add_custom_command(TARGET Snowball POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory  ${CMAKE_BINARY_DIR}/Snowball/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources
    ${CMAKE_BINARY_DIR}/Snowball/Resources)