#include "InputGLFW.h"
#include "Application.h"
#include "GLFW/glfw3.h"

namespace Skynet
{
    
    // create the derive instance class with defined override functions 
    Input *Input::instance = new InputGLFW(); 

    bool InputGLFW::IsKeyPressedImpl(uint16_t keycode) 
    {
        auto window = (GLFWwindow*)(Application::GetApplication().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool InputGLFW::IsMouseButtonPressedImpl(uint16_t button) 
    {
        auto window = (GLFWwindow*)(Application::GetApplication().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> InputGLFW::GetMousePosImpl() 
    {
        auto window = (GLFWwindow*)(Application::GetApplication().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return { (float)xPos, (float)yPos };
    }

    float InputGLFW::GetMouseXImpl() 
    {
        auto [x,y] = GetMousePosImpl();
        return x;
    }

    float InputGLFW::GetMouseYImpl() 
    {
        auto [x,y] = GetMousePosImpl();
        return y;
    }

} // namespace Skynet
