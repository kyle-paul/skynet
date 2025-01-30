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
	src/link.cpp
	src/joint.cpp
	src/mesh.cpp
	src/transformable.cpp
	src/gui.cpp
	src/frame.cpp
	src/guizmo.cpp
	src/imguizmo.cpp
	src/serializer.cpp

	vendor/glad/src/glad.c
	vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
	vendor/dialog/ImGuiFileDialog.cpp
)

include_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	libs/glfw-build/install/include
	libs/glm-build/install/include
	libs/assimp-build/install/include
	libs/tinyxml2-build/install/include
	libs/yaml-cpp-build/install/include
	vendor/dialog
	vendor/imgui
	vendor/imgui/backends
	vendor/glad/include
)

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

list(APPEND CMAKE_PREFIX_PATH libs/glfw-build/install/lib/cmake/glfw3)
list(APPEND CMAKE_PREFIX_PATH libs/glm-build/install/share/glm)
list(APPEND CMAKE_PREFIX_PATH libs/assimp-build/install/lib/cmake/assimp-5.4)
list(APPEND CMAKE_PREFIX_PATH libs/tinyxml2-build/install/lib/cmake/tinyxml2)
list(APPEND CMAKE_PREFIX_PATH libs/yaml-cpp-build/install/lib/cmake/yaml-cpp)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(${PROJECT_NAME} glfw glm::glm assimp::assimp tinyxml2::tinyxml2 
                                      yaml-cpp::yaml-cpp ${OPENGL_LIBRARIES})