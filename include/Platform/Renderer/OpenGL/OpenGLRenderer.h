#ifndef SKYNET_OPENGL_RENDERER_H
#define SKYNET_OPENGL_RENDERER_H

#include "glad/glad.h"
#include "Buffer.h"

namespace Skynet
{

    class OpenGLRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void Resize(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(float* color);
        static void ClearBufferBit();
        static void Draw(ref<VertexArray>& va);
        static void DrawLine(ref<VertexArray>& va);
        static void DrawPoint(ref<VertexArray>& va);
    };

} // namespace Skynet

#endif // SKYNET_OPENGL_RENDERER_H