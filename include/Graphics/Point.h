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
        Point(titan::vec3& p);
        Point() = default;
        ~Point() = default;

        void Submit(float* p);
        void InitGL();
        
        inline ref<VertexArray>& GetVA() { return va; }
        inline titan::vec3 GetPoint() const {
            return titan::vec3(verts);
        }

    private:
        ref<VertexArray> va;
        ref<VertexBuffer> vb;

        float verts[3];
    };

} // namespace Skynet

#endif