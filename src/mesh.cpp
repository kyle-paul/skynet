#include "mesh.h"

loader = Mesh::Loader::scratch;

Mesh::Mesh(const std::string &path) {
	switch(loader) {
		case(Load::scratch) : {
			this->readfile(path);
			break;
		}
	}
	ASSERT(false, "Have yet to implement other loader methods");

	va = cref<VertexArray>();
	vb = cref<VertexBuffer>(verts.data(), verts.size() * sizeof(float));
	vb->setLayout({
        { ShaderDataType::Float3, "a_position", true},
        { ShaderDataType::Float3, "a_normals",  true},
        { ShaderDataType::Float2, "a_texcoord", false},
    }); va->addVB(vb);
    ib = cref<IndexBuffer>(faces.data(), faces.size());
    va->setIB(ib);
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
    while ((pos = tempLine.find(delimiter)) != std::string::npos) {
        token = tempLine.substr(0, pos);
        res.push_back(token);
        tempLine.erase(0, pos + delimiter.size());
    }
    res.push_back(tempLine);
    return res;
}


void mesh::readfile(const std::string& path) {
	std::ifstream file(filepath.c_str());
    if (!file.is_open()) {
        ERROR("Failed to open file: {0}", filepath);
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