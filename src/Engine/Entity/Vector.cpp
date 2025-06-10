#include "Vector.h"

namespace Skynet
{
    Vector::Vector(titan::vec3 start, titan::vec3 end)
    {
        this->Submit(start.raw(), end.raw());
        this->InitGL();
    }

    Vector::Vector(float* start, float* end)
    {
        WARN("{} {} {}", start[0], start[1], start[2]);
        WARN("{} {} {}", end[0], end[1], end[2]);

        this->Submit(start, end);
        this->InitGL();
    }

    void Vector::Submit(float* start, float* end)
    {
        verts[0] = start[0];
        verts[1] = start[1];
        verts[2] = start[2];
        verts[3] = end[0];
        verts[4] = end[1];
        verts[5] = end[2];
        this->InitGL();
    }

    void Vector::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts, sizeof(verts));
        vb->SetLayout({{ShaderDataType::Float3, "position", true}}); 
        va->AddVB(vb);
    }

} // namespace Skynet