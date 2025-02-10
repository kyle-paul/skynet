#ifndef SKYNET_RENDERER_H
#define SKYNET_RENDERER_H

#include "System.h"
#include "Buffer.h"

enum class APIBackends
{
    OpenGL  = 0,
    Vulkan  = 1,
    DirectX = 2,
};

namespace Skynet 
{

    /* 
    This class with static functions act as 
    router to different rendering backends 
    */

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void Resize(uint32_t width, uint32_t height);
        static void SetClearColor(float* color);
        static void ClearBufferBit();
        static void Draw(ref<VertexArray>& va);
        static void DrawLine(ref<VertexArray>& va);

    private:
        APIBackends backend = APIBackends::OpenGL;
    };

} // namespace Skynet

#endif // SKYNET_RENDERER_H