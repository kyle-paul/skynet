#include "serializer.h"
#include "mesh.h"

namespace YAML {
	template<>
	struct convert<float[3]> {
		static Node encode(const float (&v)[3]) {
			if (!v) return Node();
			Node node;
			node.push_back(v[0]);
			node.push_back(v[1]);
			node.push_back(v[2]);
			return node;
		}
		static bool decode(const Node &node, float (&v)[3]) {
	        if (!node.IsSequence() || node.size() != 2) return false;
	        v[0] = node[0].as<float>();
	        v[1] = node[1].as<float>();
	        v[2] = node[2].as<float>();
	        return true;
	    }
	};

	template<>
	struct convert<float[4]> {
		static Node encode(const float (&v)[4]) {
			if (!v) return Node();
			Node node;
			node.push_back(v[0]);
			node.push_back(v[1]);
			node.push_back(v[2]);
			node.push_back(v[3]);
			return node;
		}
		static bool decode(const Node &node, float (&v)[4]) {
	        if (!node.IsSequence() || node.size() != 3) return false;
	        v[0] = node[0].as<float>();
	        v[1] = node[1].as<float>();
	        v[2] = node[2].as<float>();
	        v[3] = node[3].as<float>();
	        return true;
	    }
	};
};

YAML::Emitter& operator<<(YAML::Emitter& out, const float (&v)[3]) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v[0] << v[1] << v[2] << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const float (&v)[4]) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v[0] << v[1] << v[2] << v[3] << YAML::EndSeq;
	return out;
}


Serializer::Serializer(ref<Scene> &scene) : scene(scene) {

}

Serializer::~Serializer() {

}

void Serializer::serialize(const std::string &path) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "graphics simulation";
	out << YAML::Key << "Default" << YAML::Value << scene->robotpath;
	out << YAML::Key << "Joint" << YAML::Value << YAML::BeginSeq;
	out << YAML::EndSeq;
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	for (auto &[name, object] : scene->objects) {
		this->serializeEntity(out, name, object);
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	
	std::ofstream fout(path);
	fout << out.c_str();
}

void Serializer::serializeEntity(YAML::Emitter &out, const std::string &name, ref<Mesh> &object) {
	out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << name;
    out << YAML::Key << "transform";
	out << YAML::BeginMap;
	out << YAML::Key << "p" << YAML::Value << object->p;
	out << YAML::Key << "e" << YAML::Value << object->e;
	out << YAML::Key << "q" << YAML::Value << object->q;
	out << YAML::Key << "s" << YAML::Value << object->s;
	out << YAML::Key << "w" << YAML::Value << object->w;
	out << YAML::Key << "a" << YAML::Value << object->a;
	out << YAML::EndMap;
}

bool Serializer::deserialize(const std::string &path) {
	std::ifstream stream(path);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Scene"]) return false;

	std::string sceneName = data["Scene"].as<std::string>();
	SUCCESS("Deserializing scene '{0}'", sceneName);

	return true;
}