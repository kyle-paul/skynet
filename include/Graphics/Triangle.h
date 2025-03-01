#ifndef SKYNET_GRAPHICS_TRIANGLE
#define SKYNET_GRAPHICS_TRIANGLE

#include "System.h"
#include "Buffer.h"
#include "Titan.hpp"

namespace Skynet
{
    
    class Triangle
    {
    public:
        Triangle() = default;
        ~Triangle() = default;

        void Submit(float* p1, float* p2, float* p3);
        void InitGL();

    private:
        ref<VertexArray> va;
        ref<VertexBuffer> vb;

        float verts[9];
    };

} // namespace Skynet


#endif // SKYNET_GRAPHICS_TRIANGLE