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
        Point() = default;
        Point(titan::vec3& p);
        ~Point() = default;

        void Submit(float* p);
        void InitGL();
        
        inline ref<VertexArray>& GetVA() { return va; }

    private:
        ref<VertexArray> va;
        ref<VertexBuffer> vb;

        float verts[3];
    };

} // namespace Skynet

#endif