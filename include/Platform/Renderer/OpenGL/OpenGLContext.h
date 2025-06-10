#ifndef SKYNET_OPENGL_CONTEXT_H
#define SKYNET_OPENGL_CONTEXT_H

#include "Context.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Skynet
{
    class OpenGLContext : public Context {
    public:
        OpenGLContext(GLFWwindow* window);
        ~OpenGLContext();

        virtual void Init() override;

    private:
        GLFWwindow* window;
    };

} // namespace Skynet


#endif // SKYNET_OPENGL_CONTEXT_H