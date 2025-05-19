cmake_minimum_required(VERSION 3.8)

project(SnowEngineApp)

add_executable(SnowEngineApp "main.cpp")


target_link_libraries(SnowEngineApp
	SnowEngine
)