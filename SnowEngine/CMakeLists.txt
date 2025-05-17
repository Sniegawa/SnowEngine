cmake_minimum_required(VERSION 3.8)

add_executable(SnowEngine "main.cpp")

target_link_libraries (SnowEngine
		glfw
		libglew_static
		glm
)