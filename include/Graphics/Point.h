#ifndef SKYNET_GRAPHICS_POINT
#define SKYNET_GRAPHICS_POINT

#include "System.h"
#include "Buffer.h"
#include "Titan.hpp"

namespace Skynet
{
    class Point
    {
    public:
        Point();
        ~Point();

        void Submit(float* p);
        void InitGL();
        
        inline ref<VertexArray>& GetVA() { return va; }

    private:
        ref<VertexArray> va;
        ref<VertexBuffer> vb;

        float verts[6] = {0.0f, 0.0f, 0.0f};
    };

} // namespace Skynet

#endif