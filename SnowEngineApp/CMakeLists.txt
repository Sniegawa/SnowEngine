cmake_minimum_required(VERSION 3.8)

project(SnowEngineApp)

add_executable(SnowEngineApp "main.cpp")

target_link_libraries(SnowEngineApp
	SnowEngine
)

set(CMAKE_BUILD_TYPE Debug)

if(MSVC)
  target_compile_options(SnowEngineApp PRIVATE $<$<CONFIG:Debug>:/Zi /Od /MDd>)
endif()

add_custom_command(TARGET SnowEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory  ${CMAKE_BINARY_DIR}/SnowEngineApp/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    ${CMAKE_BINARY_DIR}/SnowEngineApp/Assets)