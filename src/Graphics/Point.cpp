#include "Point.h"

namespace Skynet
{

    Point::Point()
    {

    }

    Point::~Point()
    {
        
    }

    void Point::Submit(float* p)
    {
        verts[0] = p[0];
        verts[1] = p[1];
        verts[2] = p[2];
    }

    void Point::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts, sizeof(verts));
        vb->SetLayout({{ShaderDataType::Float3, "position", true}});
        va->AddVB(vb);
    }

} // namespace Skynet
