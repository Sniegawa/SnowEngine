cmake_minimum_required(VERSION 3.8)

project(SnowEngineApp)

add_executable(SnowEngineApp "main.cpp")


target_link_libraries(SnowEngineApp
	SnowEngine
)

add_custom_command(TARGET SnowEngineApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SnowEngine>
    $<TARGET_FILE_DIR:SnowEngineApp>
)