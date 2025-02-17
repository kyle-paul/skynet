#include "Serializer.h"
#include "Component.h"

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

namespace Skynet
{

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


    Serializer::Serializer()
    {
        
    }

    Serializer::~Serializer()
    {

    }

    void Serializer::Serialize(const std::string& path)
    {   
        // YAML::Emitter out;

        // out << YAML::BeginMap;
        // out << YAML::Key << "Scene" << YAML::Value << "Skynet: Graphics Simulation";
        // out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        // scene->bodies.each([&](auto entityID)
        // {
        //     out << YAML::BeginMap;
        //     out << YAML::Key << "EntityID" << YAML::Value << (uint32_t)entityID;
            
        //     if (scene->bodies.has<TagComp>(entityID))
        //     {
        //         out << YAML::Key << "TagComp";
        //         out << YAML::BeginMap;
        //         auto& c = scene->bodies.get<TagComp>(entityID);
        //         out << YAML::Key << "Tag" << YAML::Value << c.tag;
        //         out << YAML::EndMap;
        //     }

        //     if (scene->bodies.has<TextureComp>(entityID))
        //     {
        //         out << YAML::Key << "TextureComp";
        //         out << YAML::BeginMap;
        //         auto& c = scene->bodies.get<TextureComp>(entityID);
        //         out << YAML::Key << "Color" << YAML::Value << c.color;
        //         out << YAML::EndMap;
        //     }

        //     if (scene->bodies.has<MeshComp>(entityID))
        //     {
        //         out << YAML::Key << "MeshComp";
        //         out << YAML::BeginMap;
        //         auto& c = scene->bodies.get<MeshComp>(entityID);
        //         out << YAML::Key << "Object" << YAML::Value << ObjectToString(c.type);
        //         out << YAML::EndMap;
        //     }

        //     if (scene->bodies.has<TransformComp>(entityID))
        //     {
        //         out << YAML::Key << "TransformComp";
        //         out << YAML::BeginMap;
        //         auto& c = scene->bodies.get<TransformComp>(entityID);
        //         out << YAML::Key << "Position" << YAML::Value << c.p;
        //         out << YAML::Key << "Euler" << YAML::Value << c.e;
        //         out << YAML::Key << "Scale" << YAML::Value << c.s;
        //         out << YAML::Key << "Quaternion" << YAML::Value << c.q;
        //         out << YAML::EndMap;
        //     }

        //     if (scene->bodies.has<RigidBodyComp>(entityID))
        //     {
        //         out << YAML::Key << "RigidBodyComp";
        //         out << YAML::BeginMap;
        //         auto& c = scene->bodies.get<RigidBodyComp>(entityID);
        //         out << YAML::Key << "Linear Velocity" << YAML::Value << c.body.vel;
        //         out << YAML::Key << "Angular Velocity" << YAML::Value << c.body.omega;
        //         out << YAML::EndMap;
        //     }

        //     out << YAML::EndMap;
        // });

        // out << YAML::EndSeq;    
		// out << YAML::EndMap;

        // std::ofstream fout(path);
		// fout << out.c_str();
    }

    bool Serializer::Deserialize(const std::string& path)
    {

        // std::ifstream stream(path);
		// std::stringstream strStream;
		// strStream << stream.rdbuf();

        // YAML::Node data = YAML::Load(strStream.str());
		// if (!data["Scene"])
		// 	return false;

        // std::string sceneName = data["Scene"].as<std::string>();
		// INFO("Deserializing scene '{}'", sceneName);
    
        // auto entities = data["Entities"];

        // if (entities)
        // {
        //     for (auto entity : entities)
        //     {
        //         uint32_t ID = entity["EntityID"].as<uint32_t>();

        //         std::string name;
        //         auto tagComponent = entity["TagComp"];
        //         if (tagComponent) name = tagComponent["Tag"].as<std::string>();

        //         LOG("Deserialized entity with ID = {}, name = {}", ID, name);

        //         entt::entity body = scene->bodies.create();
        //         scene->bodies.emplace<TagComp>(body, name);

        //         auto texturecomponent = entity["TextureComp"];
        //         auto meshcomponent = entity["MeshComp"];
		// 		auto transformcomponent = entity["TransformComp"];
		// 		auto rigidbodycomponent = entity["RigidBodyComp"];

        //         if (texturecomponent) 
        //         {
        //             float color[4];
        //             YAML::convert<float[4]>::decode(texturecomponent["Color"], color);
        //             scene->bodies.emplace<TextureComp>(body, color);
        //         }

        //         if (meshcomponent)
        //         {
        //             Object type = StringToObject(meshcomponent["Object"].as<std::string>());
        //             switch(type)
        //             {
        //                 case(Object::Sphere) : scene->bodies.emplace<MeshComp>(body, type, 0.5f); break;
        //                 case(Object::Cube)   : scene->bodies.emplace<MeshComp>(body, type, 0.5f, 0.5f, 0.5f); break;
        //             }

        //             scene->bodies.get<MeshComp>(body).mesh->InitGL();
        //         }

        //         if (transformcomponent)
        //         {
        //             float p[3], e[3], s[3], q[4];
        //             YAML::convert<float[3]>::decode(transformcomponent["Position"], p);
        //             YAML::convert<float[3]>::decode(transformcomponent["Euler"], e);
        //             YAML::convert<float[3]>::decode(transformcomponent["Scale"], s);
        //             YAML::convert<float[4]>::decode(transformcomponent["Quaternion"], q);

        //             scene->bodies.emplace<TransformComp>(body, p, e, s, q);
        //         }

        //         if (rigidbodycomponent)
        //         {
        //             scene->bodies.emplace<RigidBodyComp>(body);
        //         }
        //     }
        // }

        return true;
    }

} // namespace Skynet
