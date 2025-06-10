#ifndef SKYNET_WINDOWGLFW_H
#define SKYNET_WINDOWGLFW_H

#include "Window.h"
#include "Context.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Skynet
{
    class WindowGLFW : public Window {
    public:
        WindowGLFW(const WindowProps& props);
		~WindowGLFW();

        void OnUpdate() override;

        inline void SetEventCallback(const EventCallbackFn& callback) override { 
			config.callback = callback; 
		}

        void SetVSync(bool enabled) override;
		bool IsVSync() const override;

        uint32_t GetWidth()  const override { return config.width;  }
		uint32_t GetHeight() const override { return config.height; }

        inline virtual float GetTime() const { return (float)glfwGetTime(); }
        inline virtual void* GetNativeWindow() const { return window; }

    private:
		void Init(const WindowProps& props);
		void Shutdown();

    private:
        GLFWwindow* window;
        Context* context;

        struct WindowConfig {
			bool vsync;
			std::string title;
			uint32_t width, height;
			EventCallbackFn callback;
		};

        WindowConfig config;
    };

} // namespace Skynet


#endif // SKYNET_WINDOWGLFW_H