#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core.h"
#include "gui.h"
#include "imgui_internal.h"

void OpenGLOptions() {
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

Data data;
MouseConfig& msc = data.msc;

void MsMoveCb(GLFWwindow* window, double xpos, double ypos) {
    if (!msc.viewport_hover) return;
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
    if (!msc.viewport_hover) return;
    msc.zoom = yoffset;
}

void keyCb(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        data.opt.guizmo_type = ImGuizmo::OPERATION::TRANSLATE;
    }

    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        data.opt.guizmo_type = ImGuizmo::OPERATION::ROTATE;
    }

    else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        data.opt.guizmo_type = ImGuizmo::OPERATION::SCALE;
    }

    else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        glDisable(GL_DEPTH_TEST);
    }
}

int main() {

	GLFWwindow* window;
	if (!glfwInit()) return -1;

	window = glfwCreateWindow(1500, 900, "Graphic Simulation", NULL, NULL);
	if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, MsScrollCb);
    glfwSetCursorPosCallback(window, MsMoveCb);
    glfwSetKeyCallback(window, keyCb);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ASSERT(false, "Failed to initialize GLAD\n");
        return -1;
    }

    OpenGLOptions();

    {
        ref<Scene> scene = cref<Scene>(); scene->init();
        Interface gui = Interface(window);
        while (!glfwWindowShouldClose(window)) {
            msc.viewport_hover = gui.viewport_hover;
            scene->updateData(&data);
            gui.render(scene);
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    glfwTerminate();
    return 0;
}