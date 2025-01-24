cmake_minimum_required(VERSION 3.18)
project(graphics VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/math.cpp
)

include_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	libs/glfw-build/install/include
	vendor/glad/include
)

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES})