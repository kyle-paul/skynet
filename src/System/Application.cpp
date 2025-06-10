#include "Application.h"
#include "Renderer.h"

namespace Skynet
{
    Application *Application::instance = nullptr;

    Application::Application() {
        /* Allow only one window */
        ASSERT(!instance, "Application already exits!");
        instance = this;

        /* Init abstract window */
        WindowProps props;
        window = Window::Create(props);
        window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

        /* Init renderer backends */
        Renderer::Init();

        /* Init default gui */ 
        interface = new Interface();
		this->PushOverLayer(interface);
    }

    Application::~Application() {
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer) {
        layerstack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverLayer(Layer* overlayer) {
        layerstack.PushOverLayer(overlayer);
        overlayer->OnAttach();
    }

    void Application::OnEvent(Event &event) {
        /* Close window event */
        EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent> (BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResize));

        /* Handle layer event */
		for (auto it = layerstack.rbegin(); it != layerstack.rend(); ++it) {
			if (event.handled) break;
			(*it)->OnEvent(event);
		}
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {		
		running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event) {
		if (event.GetWidth() == 0 || event.GetHeight() == 0) {
			minimized = true;
			return false;
		}
		minimized = false;
        Renderer::Resize(event.GetWidth(), event.GetHeight());
		return false;
	}

    void Application::Run() { 
        while(running) {
            float time = window->GetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time; 

            if (!minimized) {
                for (Layer* layer : layerstack) layer->OnUpdate(&timestep);
                interface->Begin();
                for (Layer* layer: layerstack) layer->OnRender();
                interface->End();
            }

            window->OnUpdate();
        }
    }

} // namespace Skynet

Skynet::Application* Skynet::CreateApplication() {
    return new Application();
}