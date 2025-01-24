cmake_minimum_required(VERSION 3.18)
project(graphics VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/scene.cpp
	src/math.cpp
	src/camera.cpp
	src/shader.cpp
	src/buffer.cpp
	src/mesh.cpp

	vendor/glad/src/glad.c
)

include_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	libs/glfw-build/install/include
	libs/glm-build/install/include
	vendor/glad/include
)

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

list(APPEND CMAKE_PREFIX_PATH libs/glfw-build/install/lib/cmake/glfw3)
list(APPEND CMAKE_PREFIX_PATH libs/glm-build/install/share/glm)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw glm::glm ${OPENGL_LIBRARIES})