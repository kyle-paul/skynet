#include "WindowGLFW.h"
#include "WindowEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "OpenGLContext.h"

namespace Skynet 
{
    static bool intialized = false;

	static void WindowErrorCallBack(int code, const char* description) {
		ERROR("GLFW Error ({}): {}", code, description);
	}

    WindowGLFW::WindowGLFW(const WindowProps& props) 
    {
        this->Init(props);
    }

	WindowGLFW::~WindowGLFW() 
	{
		this->Shutdown();
	}

    void WindowGLFW::Init(const WindowProps& props)
    {
        config.title  = props.title;
		config.width  = props.width;
		config.height = props.height;

        SUCCESS("Window {} ({}, {})", props.title, props.width, props.height);

        if (!intialized) {
			int success = glfwInit();
			ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(WindowErrorCallBack);
			intialized = true;
		}

        window = glfwCreateWindow((int)props.width, (int)props.height, config.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &config);

		context = new OpenGLContext(window);
		context->Init();

		this->SetVSync(true);

		// Resize window callback
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			t_windowConfig.callback(event);
			t_windowConfig.width  = width;
			t_windowConfig.height = height;
		});

		/* Close window callback */
		glfwSetWindowCloseCallback(window, [](GLFWwindow* window){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			t_windowConfig.callback(event);
		});

		/* Key callback */
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					t_windowConfig.callback(event);
					break;
				}
					
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					t_windowConfig.callback(event);
					break;
				}
					
				case GLFW_REPEAT: 
				{
					KeyPressedEvent event(key, 1);
					t_windowConfig.callback(event);
					break;
				}
			}
		});

		/* Mouse button callback */
		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			switch(action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					t_windowConfig.callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					t_windowConfig.callback(event);
					break;
				}
			}
		});

		/* Mouse scroll callback */
		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			t_windowConfig.callback(event);
		});

		/* Cursor callback */
		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
			WindowConfig &t_windowConfig = *(WindowConfig*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event(xpos, ypos);
			t_windowConfig.callback(event);
		});
    }

	void WindowGLFW::Shutdown() {
		delete context;
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowGLFW::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowGLFW::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		config.vsync = enabled;
	}

	bool WindowGLFW::IsVSync() const {
		return config.vsync;	
	}

} // namespace Skynet