#ifndef SKYNET_VECTOR_H
#define SKYNET_VECTOR_H

#include "System.h"
#include "Buffer.h"

namespace Skynet
{
    
    class Vector
    {
    public:
        Vector();
        ~Vector();

        void Submit(float* start, float* end);
        void InitGL();
        
        inline ref<VertexArray>& GetVA() { return va; }

        float verts[6] = {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
        };

    private:
        
        ref<VertexArray> va;
        ref<VertexBuffer> vb;
    };

} // namespace Skynet

#endif // SKYNET_VECTOR_H