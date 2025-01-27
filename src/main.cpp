#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "gui.h"

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
        if (msc.first_left) {
            msc.lastx = xpos; msc.lasty = ypos;
            msc.first_left = false;
        }

        msc.dx = xpos - msc.lastx; msc.dy = ypos - msc.lasty;
        msc.lastx = xpos; msc.lasty = ypos;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        msc.first_left = true;
    }

    if (shiftPressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (msc.first_right) {
            msc.lastx = xpos; msc.lasty = ypos;
            msc.first_right = false;
        }

        msc.dx = xpos - msc.lastx; msc.dy = ypos - msc.lasty;
        msc.lastx = xpos; msc.lasty = ypos;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        msc.first_right = true;
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

    OpenGLOptions();

    {
        Scene scene; scene.init();
        Interface gui = Interface(window);
        while (!glfwWindowShouldClose(window)) {
            scene.updateCamera(&msc);
            gui.render(&scene);
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    glfwTerminate();
    return 0;
}