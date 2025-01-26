#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "scene.h"

void OpenGLOptions() {
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

MouseConfig msc;

void MsMoveCb(GLFWwindow* window, double xpos, double ypos) {
    bool shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
                        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    if (shiftPressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (msc.first) {
            msc.lastx = xpos; msc.lasty = ypos;
            msc.first = false;
        }

        msc.dx = xpos - msc.lastx; msc.dy = ypos - msc.lasty;
        msc.lastx = xpos; msc.lasty = ypos;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        msc.first = true;
    }
}

void MsScrollCb(GLFWwindow* window, double xoffset, double yoffset) {
    msc.zoom = yoffset;
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
    glfwSetScrollCallback(window, MsScrollCb);
    glfwSetCursorPosCallback(window, MsMoveCb);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ASSERT(false, "Failed to initialize GLAD\n");
        return -1;
    }

    {
        OpenGLOptions();
        Scene scene; scene.init();
        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 1);
            scene.updateCamera(&msc);
            scene.render();
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    glfwTerminate();
    return 0;
}