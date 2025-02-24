#include "Mesh.h"
#include "Constants.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

namespace Skynet
{
    Mesh::Mesh() 
    {

    }

    Mesh::~Mesh()
    {

    }

    void Mesh::InitGL()
    {
        va = cref<VertexArray>();
        vb = cref<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
        vb->SetLayout({
            { ShaderDataType::Float3, "position", true  },
            { ShaderDataType::Float3, "normal",   true  },
            { ShaderDataType::Float2, "texcoord", false },
        }); va->AddVB(vb);
        ib = cref<IndexBuffer>(faces.data(), faces.size());
        va->AddIB(ib);
    }

    /* main functions for generation */
    void Mesh::GenMesh(const std::string& path, const Loader& loader)
    {
        switch(loader)
        {
            case(Loader::Scratch) : this->Readfile(path); return;
            case(Loader::Assimp)  : this->AssRead(path);  return;
        }
        ASSERT(false, "Mesh loader undefined");
    }
    
    void Mesh::GenCube(float x, float y, float z) 
    {
        float vertices[] = {
            -x, -y, -z,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            x, -y, -z,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            x,  y, -z,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -x,  y, -z,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

            -x, -y,  z,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
            x, -y,  z,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
            x,  y,  z,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -x,  y,  z,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            -x,  y,  z, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -x,  y, -z, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -x, -y, -z, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -x, -y,  z, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

            x,  y,  z,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            x,  y, -z,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            x, -y, -z,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            x, -y,  z,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

            -x, -y, -z,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            x, -y, -z,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            x, -y,  z,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -x, -y,  z,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

            -x,  y, -z,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            x,  y, -z,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            x,  y,  z,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -x,  y,  z,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        verts.assign(vertices, vertices + sizeof(vertices) / sizeof(float));
        faces.assign(indices, indices + sizeof(indices) / sizeof(uint32_t));
    }

    void Mesh::GenSphere(float radius, int sectors, int stacks) 
    {
        float x, y, z, xy; // Position
        float nx, ny, nz;  // Normals
        float s, t;        // Texcoords

        float sectorStep = 2.0f * PI / sectors;
        float stackStep = PI / stacks;

        // Generate vertices
        for (int i = 0; i <= stacks; ++i) 
        {
            float stackAngle = PI / 2.0f - i * stackStep;
            xy = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectors; ++j) 
            {
                float sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                z = xy * sinf(sectorAngle);

                nx = x / radius;
                ny = y / radius;
                nz = z / radius;

                s = (float)j / sectors;
                t = (float)i / stacks;

                verts.push_back(x);
                verts.push_back(y);
                verts.push_back(z);
                
                verts.push_back(nx);
                verts.push_back(ny);
                verts.push_back(nz);
                
                verts.push_back(s);
                verts.push_back(t);
            }
        }

        for (int i = 0; i < stacks; ++i) 
        {
            for (int j = 0; j < sectors; ++j) 
            {
                int first = i * (sectors + 1) + j;
                int second = first + sectors + 1;

                faces.push_back(first);
                faces.push_back(second);
                faces.push_back(first + 1);

                faces.push_back(second);
                faces.push_back(second + 1);
                faces.push_back(first + 1);
            }
        }
    }

    /* util functions */
    void Mesh::Readfile(const std::string & path) 
    {
        std::ifstream file(path.c_str());
        if (!file.is_open()) 
        {
            ERROR("Failed to open file: {0}", path);
        }

        std::string line;
        std::vector<std::string> words;
        words.reserve(8);

        int norm_idx = 0;
        int text_idx = 0;

        while(std::getline(file, line)) 
        {
            line.erase(0, line.find_first_not_of(" \t"));
            if (line.empty() || line[0] == '#')  continue;

            if (line.substr(0, 2) == "v ") 
            {
                words.clear(); this->Split(line, " ", words);
                if (words.empty()) continue;

                verts.push_back(std::stof(words[1]));
                verts.push_back(std::stof(words[2]));
                verts.push_back(std::stof(words[3]));

                verts.push_back(0.0f);
                verts.push_back(0.0f);
                verts.push_back(0.0f);

                verts.push_back(0.0f);
                verts.push_back(0.0f);
            }

            else if (line.substr(0, 2) == "vn") 
            {
                words.clear(); this->Split(line, " ", words);
                if (words.empty()) continue;

                verts[norm_idx*8 + 3] = std::stof(words[1]);
                verts[norm_idx*8 + 4] = std::stof(words[2]);
                verts[norm_idx*8 + 5] = std::stof(words[3]);
                norm_idx++;
            }

            else if (line.substr(0, 2) == "vt") 
            {
                words.clear(); this->Split(line, " ", words);
                if (words.empty()) continue;

                verts[text_idx*8 + 6] = std::stof(words[1]);
                verts[text_idx*8 + 7] = std::stof(words[2]);
                text_idx++;
            }

            else if (line[0] == 'f') 
            {
                words.clear(); this->Split(line, " ", words);
                if (words.empty()) continue;

                for (size_t i = 1; i < words.size(); ++i) 
                {
                    std::vector<std::string> parts;
                    this->Split(words[i], "/", parts);

                    if (!parts.empty() && !parts[0].empty()) 
                    {
                        faces.push_back(std::stoi(parts[0]) - 1);
                    }
                }

            }
        }
    }

    void Mesh::AssRead(const std::string & path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            ERROR("Failed to load mesh: {0}", importer.GetErrorString());
        }

        for (size_t i = 0; i < scene->mNumMeshes; ++i) 
        {
            const aiMesh* mesh = scene->mMeshes[i];
            num_verts =  mesh->mNumVertices;

            for (size_t j = 0; j < num_verts; ++j)
            {
                float x = mesh->mVertices[j].x;
                float y = mesh->mVertices[j].y;
                float z = mesh->mVertices[j].z;

                // Vertex positions
                verts.push_back(x);
                verts.push_back(y);
                verts.push_back(z);

                // Vertex normals
                verts.push_back(mesh->mNormals[j].x);
                verts.push_back(mesh->mNormals[j].y);
                verts.push_back(mesh->mNormals[j].z);

                // Vertex texture
                if (mesh->mTextureCoords[0]) 
                {
                    verts.push_back(mesh->mTextureCoords[0][j].x);
                    verts.push_back(mesh->mTextureCoords[0][j].y);
                } 
                else 
                {
                    verts.push_back(0.0f);
                    verts.push_back(0.0f);
                }
            }   

            for (size_t j = 0; j < mesh->mNumFaces; ++j) 
            {
                const aiFace& face = mesh->mFaces[j];

                for (size_t k = 0; k < face.mNumIndices; ++k) 
                {
                    faces.push_back(face.mIndices[k]);
                }
            }
        }
    }

    std::vector<std::string> Mesh::Split(
        const std::string& line, const std::string& del, 
        std::vector<std::string>& res) 
    {
        size_t pos = 0;
        std::string token;
        std::string tempLine = line;
        while ((pos = tempLine.find(del)) != std::string::npos) 
        {
            token = tempLine.substr(0, pos);
            res.push_back(token);
            tempLine.erase(0, pos + del.size());
        }
        res.push_back(tempLine);
        return res;
    }

    void Mesh::Info() 
    {
        for (int i=0; i < verts.size(); i++) 
        {
            std::cout << verts[i] << ' ';
            if ((i + 1) % 8 == 0) std::cout << '\n';
        }

        for (int i=0; i < faces.size(); i++) 
        {
            std::cout << faces[i] << ' ';
            if ((i + 1) % 3 == 0)  std::cout << '\n';
        }
    }

} // namespace Skynet
