#pragma once

#include "core.h"
#include "link.h"
#include <tinyxml2.h>

#include "xml.h"
#include "math.h"

namespace xml {

static struct Data {
    std::string baseDir;
    std::unordered_map<std::string, std::string> materialMap;
    std::unordered_map<std::string, std::vector<std::string>>* graph;
    std::unordered_map<std::string, ref<Joint>>* joints;
    std::unordered_map<std::string, ref<Link>>* links;
} data;

inline void setData(std::unordered_map<std::string, std::vector<std::string>> *graph,
                    std::unordered_map<std::string, ref<Joint>> *joints,
                    std::unordered_map<std::string, ref<Link>> *links) 
{
    data.graph  = graph;
    data.joints = joints;
    data.links  = links;
}

inline void parseBody(tinyxml2::XMLElement* body) {
    // body
    float pos[3]   = {0.0f, 0.0f, 0.0f};
    float quat[4]  = {1.0f, 0.0f, 0.0f, 0.0f};
    float axis[3]  = {0.0f, 0.0f, 1.0f};
    float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    if (body->Attribute("pos"))  sscanf(body->Attribute("pos"), "%f %f %f", &pos[0], &pos[1], &pos[2]);
    if (body->Attribute("quat")) sscanf(body->Attribute("quat"), "%f %f %f %f", &quat[0], &quat[1], &quat[2], &quat[3]);

    std::string bodyName = body->Attribute("name");
    std::vector<MeshData> meshes;

    for (tinyxml2::XMLElement* geom = body->FirstChildElement("geom"); 
        geom; geom = geom->NextSiblingElement("geom"))
    {
        std::string meshPath = data.baseDir + "/assets/" + geom->Attribute("mesh") + ".obj";
        sscanf(data.materialMap[geom->Attribute("material")].c_str(), "%f %f %f %f", 
               &color[0], &color[1], &color[2], &color[3]);
        meshes.push_back({meshPath, color});
    }

    // joint
    tinyxml2::XMLElement* jointElement = body->FirstChildElement("joint");

    if (jointElement) {
        std::string jointName = jointElement->Attribute("name");
        if (jointElement->Attribute("axis")) sscanf(jointElement->Attribute("axis"), "%f %f %f", &axis[0], &axis[1], &axis[2]);
        (*data.joints)[jointName] = cref<Joint>(axis);
        (*data.links)[bodyName] = cref<Link>(pos, quat, meshes, (*data.joints)[jointName]);
    } else {
        (*data.links)[bodyName] = cref<Link>(pos, quat, meshes);
    }

    // recursive load
    for (tinyxml2::XMLElement* childBody = body->FirstChildElement("body"); 
        childBody; childBody = childBody->NextSiblingElement("body")) 
    {
        (*data.graph)[bodyName].push_back(childBody->Attribute("name"));
        parseBody(childBody);
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
        parseBody(body);
    }
}

}