#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "camera.h"
#include "shader.h"
#include "buffer.h"
#include "mesh.h"


void renderScene() {
    
}

int main() {

	GLFWwindow* window;
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(1200, 800, "Graphic Simulation", NULL, NULL);
	if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        renderScene();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}