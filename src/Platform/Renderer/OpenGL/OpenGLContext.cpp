#include "System.h"
#include "OpenGLContext.h"

namespace Skynet
{
    OpenGLContext::OpenGLContext(GLFWwindow* window) : window(window) {
        
    }
    
    OpenGLContext::~OpenGLContext() {

    }

    void OpenGLContext::Init() {
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Failed to initialize GLAD with GLFW (OPENGL)");

        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* device = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);

        INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(version));
        INFO("OpenGL Rendering Device is: {0} - {1}", 
                    reinterpret_cast<const char*>(vendor), 
                    reinterpret_cast<const char*>(device));

        /* Check if the vendor is NVIDIA */
        if (vendor && strstr(reinterpret_cast<const char*>(vendor), "NVIDIA") != nullptr) {
            INFO("NVIDIA Graphics Card Detected");
        } 
        else {
            INFO("Using Default Intel Graphics Card");
        }
    }
}