#ifndef SKYNET_XML_H
#define SKYNET_XML_H

#include "core.h"
#include "link.h"
#include "math.h"

#include <tinyxml2.h>


namespace xml {

static struct Data {
    std::string baseDir;
    std::unordered_map<std::string, std::string> materialMap;
    std::unordered_map<std::string, std::vector<std::string>>* graph;
    std::unordered_map<std::string, ref<Joint>>* joints;
    std::unordered_map<std::string, ref<Link>>* links;
    std::unordered_map<std::string, float[3]>* u;
    std::unordered_map<std::string, float>* m;
    std::unordered_map<std::string, std::string>* dof_parent;
    uint32_t index = 0;
} data;

inline void setData(std::unordered_map<std::string, std::vector<std::string>>* graph,
                    std::unordered_map<std::string, ref<Joint>>* joints,
                    std::unordered_map<std::string, ref<Link>>* links, 
                    std::unordered_map<std::string, float[3]>* u,
                    std::unordered_map<std::string, float>* m,
                    std::unordered_map<std::string, std::string>* dof_parent)
{
    data.graph  = graph;
    data.joints = joints;
    data.links  = links;
    data.u = u;
    data.m = m;
    data.dof_parent = dof_parent;
}

inline void parseBody(tinyxml2::XMLElement* body, const std::string &cur_dof_name) {
    // body
    float pos[3]   = {0.0f, 0.0f, 0.0f};
    float quat[4]  = {1.0f, 0.0f, 0.0f, 0.0f};
    float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    
    if (body->Attribute("pos"))  sscanf(body->Attribute("pos"), "%f %f %f", &pos[0], &pos[1], &pos[2]);
    if (body->Attribute("quat")) sscanf(body->Attribute("quat"), "%f %f %f %f", &quat[0], &quat[1], &quat[2], &quat[3]);

    std::string bodyName = body->Attribute("name");
    std::vector<MeshData> meshes;

    memcpy((*data.u)[bodyName], pos, sizeof(pos));
    (*data.m)[bodyName] = 0.0f;

    for (tinyxml2::XMLElement* geom = body->FirstChildElement("geom"); 
        geom; geom = geom->NextSiblingElement("geom"))
    {
        std::string meshPath = data.baseDir + "/assets/" + geom->Attribute("mesh") + ".obj";
        sscanf(data.materialMap[geom->Attribute("material")].c_str(), "%f %f %f %f", 
               &color[0], &color[1], &color[2], &color[3]);
        meshes.push_back({meshPath, color});
    }

    // inerial
    float com[3] = {0.0f, 0.0f, 0.0f};
    float mass   = 0.0f;

    tinyxml2::XMLElement* inertialElement = body->FirstChildElement("inertial");
    if (!inertialElement) ASSERT(false, "Loading XML failed: body must have intertial configuration");

    ASSERT(inertialElement->Attribute("mass"), "Loading XML failed: body mass must be specified");
    ASSERT(inertialElement->Attribute("pos"),  "Loading XML failed: body center of mass must be specified");

    sscanf(inertialElement->Attribute("mass"), "%f", &mass);
    sscanf(inertialElement->Attribute("pos"),  "%f %f %f", &com[0], &com[1], &com[2]);

    // joint
    float axis[3]    = {0.0f, 0.0f, 1.0f};
    float range[2]   = {-300.0f, 300.0f};
    JType joint_type = JType::Hinge;

    tinyxml2::XMLElement* jointElement = body->FirstChildElement("joint");
    bool has_joint = false;
    std::string jointName;

    if (jointElement) {
        jointName = jointElement->Attribute("name");
        if (jointElement->Attribute("axis"))  sscanf(jointElement->Attribute("axis"), "%f %f %f", &axis[0], &axis[1], &axis[2]);
        if (jointElement->Attribute("type"))  joint_type = StringToJType(jointElement->Attribute("type"));
        if (jointElement->Attribute("range")) sscanf(jointElement->Attribute("range"), "%f %f", &range[0], &range[1]);

        (*data.joints)[jointName]     = cref<Joint>(data.index++, joint_type, range, axis);
        (*data.links)[bodyName]       = cref<Link>(pos, quat, mass, com, meshes, (*data.joints)[jointName]);
        (*data.dof_parent)[jointName] = cur_dof_name;

    } else {
        (*data.links)[bodyName] = cref<Link>(pos, quat, mass, com, meshes);
        jointName = cur_dof_name;
    }

    // recursive load
    for (tinyxml2::XMLElement* childBody = body->FirstChildElement("body"); 
        childBody; childBody = childBody->NextSiblingElement("body")) 
    {
        (*data.graph)[bodyName].push_back(childBody->Attribute("name"));
        parseBody(childBody, jointName);
    }
}

inline void parseXML(const std::string &path) 
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        ERROR("Error loading XML file: {0}", path);
        return;
    }

    data.baseDir = path.substr(0, path.find_last_of('/'));

    // sanity checks
    tinyxml2::XMLElement* root = doc.FirstChildElement("vortex");
    if (!root) ASSERT(false, "Invalid XML: Missing <vortex> root element");

    tinyxml2::XMLElement* worldbody = root->FirstChildElement("worldbody");
    if (!worldbody) ASSERT(false, "No <worldbody> element found!");

    // mapping
    tinyxml2::XMLElement* asset = root->FirstChildElement("asset");
    if (asset) {
        for (tinyxml2::XMLElement* material = asset->FirstChildElement("material");
            material; material = material->NextSiblingElement("material")) {
            std::string name = material->Attribute("name");
            std::string rgba = material->Attribute("rgba");
            data.materialMap[name] = rgba;
        }
    } else {ASSERT(false, "No <asset> element found!");}

    // recursive load
    for (tinyxml2::XMLElement* body = worldbody->FirstChildElement("body"); 
        body; body = body->NextSiblingElement("body"))
    {
        (*data.dof_parent)[body->Attribute("name")] = "null";
        parseBody(body, "null");
    }
}

} // namespace xml

#endif // SKYNET_XML_H