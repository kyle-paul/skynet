#include "serializer.h"
#include "mesh.h"
#include "xml.h"

namespace YAML {
    template <size_t N>
    struct convert<float[N]> {
        static Node encode(const float (&v)[N]) {
            Node node;
            for (size_t i = 0; i < N; ++i) {
                node.push_back(v[i]);
            }
            return node;
        }

        static bool decode(const Node& node, float (&v)[N]) {
            if (!node.IsSequence() || node.size() != N) return false;
            for (size_t i = 0; i < N; ++i) {
                v[i] = node[i].as<float>();
            }
            return true;
        }
    };
}

YAML::Emitter& operator<<(YAML::Emitter& out, const float (&v)[2]) {
	out << YAML::Flow;
	out << YAML::BeginSeq << v[0] << v[1] << YAML::EndSeq;
	return out;
}

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


Serializer::Serializer() {

}

Serializer::~Serializer() {

}

void Serializer::serialize(const std::string &path) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "graphics simulation";
	out << YAML::Key << "Default" << YAML::Value << scene->robotpath;

	out << YAML::Key << "Joints" << YAML::Value << YAML::BeginSeq;
		for (auto &[name, joint] : scene->joints) {
			out << YAML::BeginMap;
			out << YAML::Key << "joint" << YAML::Value << name;
			out << YAML::Key << "configuration";
			out << YAML::BeginMap;
			out << YAML::Key << "type" << YAML::Value <<  JTypeToString(joint->type);
			out << YAML::Key << "range" << YAML::Value << joint->range;
			out << YAML::Key << "w" << YAML::Value << joint->w;
			out << YAML::Key << "a" << YAML::Value << joint->a;
			out << YAML::EndMap;
			out << YAML::EndMap;
		}
	out << YAML::EndSeq;

	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto &[name, object] : scene->objects) {
			out << YAML::BeginMap;
			out << YAML::Key << "entity" << YAML::Value << name;
			out << YAML::Key << "type" << YAML::Value << ObjectToString(object->getType());
			out << YAML::Key << "color" << YAML::Value << object->color;
			out << YAML::Key << "transform";
			out << YAML::BeginMap;
			out << YAML::Key << "p" << YAML::Value << object->p;
			out << YAML::Key << "e" << YAML::Value << object->e;
			out << YAML::Key << "q" << YAML::Value << object->q;
			out << YAML::Key << "s" << YAML::Value << object->s;
			out << YAML::Key << "w" << YAML::Value << object->w;
			out << YAML::Key << "a" << YAML::Value << object->a;
			out << YAML::EndMap;
			out << YAML::EndMap;

		}
	out << YAML::EndSeq;
	out << YAML::EndMap;
	
	std::ofstream fout(path);
	fout << out.c_str();
}

bool Serializer::deserialize(const std::string &path) {
	std::ifstream stream(path);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Scene"]) return false;

	std::string sceneName = data["Scene"].as<std::string>();
	SUCCESS("Deserializing scene '{0}'", sceneName);

	// load xml default robot
	std::string xml_path = data["Default"].as<std::string>();
	scene->load(xml_path);

	// load joints configuration
	auto joints = data["Joints"];
	if (joints) {
		for (auto joint : joints) {
			std::string joint_name = joint["joint"].as<std::string>();
			auto configuration = joint["configuration"];

			scene->joints[joint_name]->type = StringToJType(configuration["type"].as<std::string>());
			YAML::convert<float[2]>::decode(configuration["range"], scene->joints[joint_name]->range);
			YAML::convert<float[3]>::decode(configuration["w"], scene->joints[joint_name]->w);
			scene->joints[joint_name]->a = configuration["a"].as<float>();
		}
	}

	// load normal entities
	auto entities = data["Entities"];
	if (entities) {
		for (auto entity : entities) {
			std::string entity_name = entity["entity"].as<std::string>();
			scene->create(StringToObject(entity["type"].as<std::string>()), entity_name);
			YAML::convert<float[4]>::decode(entity["color"], scene->objects[entity_name]->color);

			auto transform = entity["transform"];
			YAML::convert<float[3]>::decode(transform["p"], scene->objects[entity_name]->p);
			YAML::convert<float[3]>::decode(transform["e"], scene->objects[entity_name]->e);
			YAML::convert<float[4]>::decode(transform["q"], scene->objects[entity_name]->q);
			YAML::convert<float[3]>::decode(transform["s"], scene->objects[entity_name]->s);
			YAML::convert<float[3]>::decode(transform["w"], scene->objects[entity_name]->w);
			scene->objects[entity_name]->a = transform["a"].as<float>();
		}
	}

	return true;
}