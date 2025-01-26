#include "mesh.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


Mesh::Loader Mesh::loader = Mesh::Loader::scratch;

Mesh::Mesh(const std::string &path) {
	switch(loader) {
		case(Loader::scratch) : {
			this->readfile(path); break;
		}
        case(Loader::assimp) : {
            this->assRead(path); break;
        }
	}

	va = cref<VertexArray>();
	vb = cref<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
	vb->setLayout({
        { ShaderDataType::Float3, "a_position", true},
        { ShaderDataType::Float3, "a_normal",  true},
        { ShaderDataType::Float2, "a_texcoord", false},
    }); va->addVB(vb);
    ib = cref<IndexBuffer>(faces.data(), faces.size());
    va->addIB(ib);
}

Mesh::~Mesh() {

}

std::vector<std::string> Mesh::split(
	const std::string &line, const std::string &del, 
	std::vector<std::string> &res) 
{
    size_t pos = 0;
    std::string token;
    std::string tempLine = line;
    while ((pos = tempLine.find(del)) != std::string::npos) {
        token = tempLine.substr(0, pos);
        res.push_back(token);
        tempLine.erase(0, pos + del.size());
    }
    res.push_back(tempLine);
    return res;
}


void Mesh::readfile(const std::string& path) {
	std::ifstream file(path.c_str());
    if (!file.is_open()) {
        ERROR("Failed to open file: {0}", path);
    }

    std::string line;
    std::vector<std::string> words;
    words.reserve(8);

    int norm_idx = 0;
    int text_idx = 0;

    while(std::getline(file, line)) {

        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty() || line[0] == '#')  continue;

        if (line.substr(0, 2) == "v ") {
            words.clear(); this->split(line, " ", words);
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

        else if (line.substr(0, 2) == "vn") {
            words.clear(); this->split(line, " ", words);
            if (words.empty()) continue;

            verts[norm_idx*8 + 3] = std::stof(words[1]);
            verts[norm_idx*8 + 4] = std::stof(words[2]);
            verts[norm_idx*8 + 5] = std::stof(words[3]);
            norm_idx++;
        }

        else if (line.substr(0, 2) == "vt") {
            words.clear(); this->split(line, " ", words);
            if (words.empty()) continue;

            verts[text_idx*8 + 6] = std::stof(words[1]);
            verts[text_idx*8 + 7] = std::stof(words[2]);
            text_idx++;
        }

        else if (line[0] == 'f') {
            words.clear(); this->split(line, " ", words);
            if (words.empty()) continue;

            for (size_t i = 1; i < words.size(); ++i) {
                std::vector<std::string> parts;
                this->split(words[i], "/", parts);

                if (!parts.empty() && !parts[0].empty()) {
                    faces.push_back(std::stoi(parts[0]) - 1);
                }
            }
        }
    }
}


void Mesh::assRead(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        ERROR("Failed to load mesh: {0}", importer.GetErrorString());
    }

    for (size_t i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];
        for (size_t j = 0; j < mesh->mNumVertices; ++j) {
            verts.push_back(mesh->mVertices[j].x);
            verts.push_back(mesh->mVertices[j].y);
            verts.push_back(mesh->mVertices[j].z);

            verts.push_back(mesh->mNormals[j].x);
            verts.push_back(mesh->mNormals[j].y);
            verts.push_back(mesh->mNormals[j].z);

            if (mesh->mTextureCoords[0]) {
                verts.push_back(mesh->mTextureCoords[0][j].x);
                verts.push_back(mesh->mTextureCoords[0][j].y);
            } else {
                verts.push_back(0.0f);
                verts.push_back(0.0f);
            }
        }   

        for (size_t j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (size_t k = 0; k < face.mNumIndices; ++k) {
                faces.push_back(face.mIndices[k]);
            }
        }
    }
}

void Mesh::render() {
    va->bind();
    glDrawElements(GL_TRIANGLES, va->getIB()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::info() {
    for (int i=0; i<verts.size(); i++) {
        std::cout << verts[i] << ' ';
        if ((i+1) % 8==0)
            std::cout << '\n';
    }

    for (int i=0; i < faces.size(); i++) {
        std::cout << faces[i] << ' ';
        if ((i+1) % 3==0) 
            std::cout << '\n';
    }
}