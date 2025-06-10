#ifndef SKYNET_ENGINE_ENTITY_TRIANGLE
#define SKYNET_ENGINE_ENTITY_TRIANGLE

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


#endif // SKYNET_ENGINE_ENTITY_TRIANGLE