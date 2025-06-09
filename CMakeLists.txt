cmake_minimum_required (VERSION 3.8)

project(SnowEngineProject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Libs/GLAD)
add_subdirectory(Libs/glfw)
add_subdirectory(Libs/spdlog)

# glm is header-only so only include_directories needed
include_directories(Libs/glm)

# Engine core
add_subdirectory(SnowEngine)

# App
add_subdirectory(SnowEngineApp)

