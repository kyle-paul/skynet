#ifndef SKYNET_MESH_H
#define SKYNET_MESH_H

#include "Data.h"
#include "System.h"
#include "Buffer.h"
#include "Titan.hpp"

namespace Skynet
{
    struct Triangle
    {
        titan::vec3 points[3];
        titan::vec3 GetCenter() {
            return (points[0] + points[1] + points[2]) * (1.0f / 3.0f);
        }

        Triangle() = default;
        Triangle(titan::vec3 p1, titan::vec3 p2, titan::vec3 p3) {
            points[0] = p1;
            points[1] = p2;
            points[2] = p3;
        }

        void Log() {
            points[0].log();
            points[1].log();
            points[2].log();
            std::cout << '\n';
        }
    };

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
        
        list<float> verts;
        list<uint32_t> faces;

        ref<VertexArray>  va;
        ref<VertexBuffer> vb;
        ref<IndexBuffer>  ib;

    };

} // namespace Skynet


#endif // SKYNET_MESH_H