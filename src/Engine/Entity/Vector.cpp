#include "Entity/Vector.h"
#include "Math.h"

namespace Skynet
{

    Vector::Vector()
    {

    }

    Vector::~Vector()
    {

    }


    void Vector::Submit(float* start, float* end)
    {
        Math::Copy3(this->verts, start);
        Math::Copy3(this->verts + 3, end);
    }

    void Vector::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts, sizeof(verts));
        vb->SetLayout({{ShaderDataType::Float3, "position", true}}); 
        va->AddVB(vb);
    }

} // namespace Skynet