#include "Vector.h"

namespace Skynet
{
    void Vector::Submit(float* start, float* end)
    {
        verts[0] = start[0];
        verts[1] = start[1];
        verts[2] = start[2];
        verts[3] = end[0];
        verts[4] = end[1];
        verts[5] = end[2];
    }

    void Vector::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts, sizeof(verts));
        vb->SetLayout({{ShaderDataType::Float3, "position", true}}); 
        va->AddVB(vb);
    }

} // namespace Skynet