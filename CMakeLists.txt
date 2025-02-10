cmake_minimum_required(VERSION 3.18)
project(graphics VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/System/Renderer.cpp
	src/System/Interface.cpp
	src/System/Layer.cpp
	src/System/LayerStack.cpp
	src/System/Window.cpp
	src/System/Application.cpp

	src/Graphics/Buffer.cpp
	src/Graphics/Shader.cpp
	src/Graphics/Texture.cpp
	src/Graphics/Frame.cpp
	
	src/Math/Graphics.cpp
	src/Math/Vector.cpp
	src/Math/Matrix.cpp
	src/Math/Visualize.cpp
	
	src/Engine/Scene.cpp
	src/Engine/Camera.cpp
	src/Engine/Serializer.cpp

	src/Engine/Entity/Mesh.cpp
	src/Engine/Entity/Vector.cpp
	src/Engine/Entity/RigidBody.cpp
	
	src/Engine/Interface/Global.cpp
	src/Engine/Interface/Guizmo.cpp
	src/Engine/Interface/ImGuizmo.cpp
	src/Engine/Interface/FileDialog.cpp

	src/Platform/Window/WindowGLFW.cpp
	src/Platform/Window/InputGLFW.cpp
	src/Platform/Renderer/OpenGL/OpenGLContext.cpp
	src/Platform/Renderer/OpenGL/OpenGLRenderer.cpp

	vendor/glad/src/glad.c
	vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
	vendor/imgui/imgui_draw.cpp
	vendor/imgui/imgui_tables.cpp
	vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
)

include_directories(${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/include/System
	${CMAKE_CURRENT_SOURCE_DIR}/include/Event
	${CMAKE_CURRENT_SOURCE_DIR}/include/Math
	${CMAKE_CURRENT_SOURCE_DIR}/include/Engine
	${CMAKE_CURRENT_SOURCE_DIR}/include/Engine/Interface
	${CMAKE_CURRENT_SOURCE_DIR}/include/Engine/Entity
	${CMAKE_CURRENT_SOURCE_DIR}/include/Graphics
	${CMAKE_CURRENT_SOURCE_DIR}/include/Platform/Window
	${CMAKE_CURRENT_SOURCE_DIR}/include/Platform/Renderer/OpenGL

	libs/glfw-build/install/include
	libs/glm-build/install/include
	libs/assimp-build/install/include
	libs/tinyxml2-build/install/include
	libs/yaml-cpp-build/install/include

	vendor/stb
	vendor/imgui
	vendor/imgui/backends
	vendor/glad/include
	vendor/entt/include
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