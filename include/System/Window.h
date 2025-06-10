#ifndef SKYNET_WINDOW_H
#define SKYNET_WINDOW_H

#include "System.h"
#include "Event.h"

namespace Skynet
{
    struct WindowProps {
		uint32_t width;
		uint32_t height;
    	std::string title;

		WindowProps(const std::string& title = "Physics-based Graphics Simulation",
			        uint32_t width  = 1500,
			        uint32_t height = 900)
			: title(title), width(width), height(height)
		{

		}
	};

    /* Interface window abstraction for cross platform*/
    class Window {
	public:
        using EventCallbackFn = std::function<void(Event&)>;
        virtual ~Window() = default;
		virtual void OnUpdate() = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

        virtual uint32_t GetWidth()  const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual float GetTime() const = 0;
        virtual void* GetNativeWindow() const = 0;

		static scope<Window> Create(const WindowProps& props = WindowProps());
    };
};

#endif