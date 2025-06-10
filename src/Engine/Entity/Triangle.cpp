#include "Triangle.h"

namespace Skynet
{

    void Triangle::Submit(float* p1, float* p2, float* p3)
    {
        verts[0] = p1[0];
        verts[1] = p1[1];
        verts[2] = p1[2];

        verts[3] = p2[0];
        verts[4] = p2[1];
        verts[5] = p2[2];

        verts[6] = p3[0];
        verts[7] = p3[1];
        verts[8] = p3[2];
    }

    void Triangle::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts, sizeof(verts));
        vb->SetLayout({{ShaderDataType::Float3, "position", true}});
        va->AddVB(vb);
    }

} // namespace Skynet
