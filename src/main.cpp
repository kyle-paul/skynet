#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "scene.h"

void OpenGLOptions() {
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
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
        ASSERT(false, "Failed to initialize GLAD\n");
        return -1;
    }

    OpenGLOptions();

    {
        Scene scene; scene.init();
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);
            scene.render();
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    glfwTerminate();
    return 0;
}