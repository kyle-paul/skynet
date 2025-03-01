#include "Renderer.h"
#include "OpenGLRenderer.h"

namespace Skynet
{
    /* 
    At current time, assume only use 
    OpenGL. I will add more backends
    implementation in the future.
    */

    void Renderer::Init()
    {
        OpenGLRenderer::Init();
    }

    void Renderer::Shutdown()
    {
        OpenGLRenderer::Shutdown();
    }

    void Renderer::Resize(uint32_t width, uint32_t height)
    {
        OpenGLRenderer::Resize(0, 0, width, height);
    }

    void Renderer::SetClearColor(float* color)
    {
        OpenGLRenderer::SetClearColor(color);
    }

    void Renderer::ClearBufferBit()
    {
        OpenGLRenderer::ClearBufferBit();
    }

    void Renderer::Draw(ref<VertexArray>& va)
    {
        OpenGLRenderer::Draw(va);
    }

    void Renderer::DrawLine(ref<VertexArray>& va)
    {
        OpenGLRenderer::DrawLine(va);
    }

    void Renderer::DrawPoint(ref<VertexArray>& va)
    {
        OpenGLRenderer::DrawPoint(va);
    }

} // namespace Skynet