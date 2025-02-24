#include "OpenGLRenderer.h"

namespace Skynet
{

    void OpenGLRenderer::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderer::Shutdown()
    {

    }

    void OpenGLRenderer::Resize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderer::SetClearColor(float* color)
    {
        glClearColor(color[0], color[1], color[2], color[3]);
    }

    void OpenGLRenderer::ClearBufferBit()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::Draw(ref<VertexArray>& va)
    {
        va->Bind();
        glDrawElements(GL_TRIANGLES, va->GetIB()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderer::DrawLine(ref<VertexArray>& va)
    {
        va->Bind();
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, 6);
        glLineWidth(1.0f);
    }   

} // namespace Skynet
