#include "Window.h"
#include "WindowGLFW.h"

namespace Skynet
{

    scope<Window> Window::Create(const WindowProps& props) {
        return cscope<WindowGLFW>(props);
    }

} // namespace Skynet