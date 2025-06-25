cmake_minimum_required(VERSION 3.8)

project(SnowEngineApp)

add_executable(SnowEngineApp "main.cpp")

target_link_libraries(SnowEngineApp
	SnowEngine
)


add_custom_command(TARGET SnowEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory  ${CMAKE_BINARY_DIR}/SnowEngineApp/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    ${CMAKE_BINARY_DIR}/SnowEngineApp/Assets)