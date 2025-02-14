#ifndef SKYNET_MESH_H
#define SKYNET_MESH_H

#include "Data.h"
#include "System.h"
#include "Buffer.h"

namespace Skynet
{
    /*
    This class only has one mission of generating verts, 
    faces for whatever objects. No texture, color holder,
    no rendering function here.
    */

    class Mesh
    {
    public:
        Mesh();
        ~Mesh();

        /* Used for multithreading */
        void InitGL();
        void Info();

        inline ref<VertexArray>& GetVA() { return va; }
    
    public:
        void GenMesh(const std::string& path, const Loader& loader);
        void GenCube(float halfx, float halfy, float halfz);
        void GenSphere(float radius, int sectors=32, int stacks=32);

    private:
        /* util functions */
        void Readfile(const std::string& path);
        void AssRead(const std::string& path);

        std::vector<std::string> Split(const std::string& line,
			const std::string& del, std::vector<std::string>& res);

    public:
        float num_verts;
        std::vector<float> verts;
        std::vector<uint32_t> faces;

        ref<VertexArray>  va;
        ref<VertexBuffer> vb;
        ref<IndexBuffer>  ib;

    };

} // namespace Skynet


#endif // SKYNET_MESH_H