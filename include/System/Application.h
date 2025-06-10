#ifndef SKYNET_APPLICATION_H
#define SKYNET_APPLICATION_H

#include "System.h"
#include "Renderer.h"
#include "Window.h"
#include "WindowEvent.h"
#include "LayerStack.h"
#include "Interface.h"

namespace Skynet
{
    class Application {
    public:
        Application();
        ~Application();
        
        void Run();
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overlayer);

        inline static Application& GetApplication() { return *instance; } 
		inline Window& GetWindow() { return *window; }

    private:
        bool OnWindowClose(WindowCloseEvent& event);
        bool OnWindowResize(WindowResizeEvent& event);

    private:
        bool running = true;
        bool minimized = false;

        /* 
           Make sure layer and window
           are at the same level in one
           application to avoid conflicts. 
        */ 

        /* window interface */
        scope<Window> window;

        /* Layer architecture */
        LayerStack layerstack;

        /* Ensure only one instance */
        static Application* instance;

        Interface* interface;
        
        float lastFrameTime = 0.0f;
    };

    Application* CreateApplication();

} // namespace Skynet


#endif // SKYNET_APPLICATION_H