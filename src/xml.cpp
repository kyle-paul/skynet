#include "xml.h"
#include "math.h"

namespace xml {

std::unordered_map<std::string, std::string> materialMap;
std::string baseDir;

void parseBody(tinyxml2::XMLElement* body,
               std::unordered_map<std::string, std::vector<std::string>> &graph,
               std::unordered_map<std::string, ref<Link>> &links) 
{
    float pos[3]   = {0.0f, 0.0f, 0.0f};
    float quat[4]  = {1.0f, 0.0f, 0.0f, 0.0f};
    float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    if (body->Attribute("pos")) {
        sscanf(body->Attribute("pos"), "%f %f %f", &pos[0], &pos[1], &pos[2]);
    }

    if (body->Attribute("quat")) {
        sscanf(body->Attribute("quat"), "%f %f %f %f", &quat[0], &quat[1], &quat[2], &quat[3]);
        math::normVec4(quat);
    }

    std::string bodyName = body->Attribute("name");
    std::vector<MeshData> meshes;

    for (tinyxml2::XMLElement* geom = body->FirstChildElement("geom"); 
        geom; geom = geom->NextSiblingElement("geom"))
    {
        std::string meshPath = baseDir + "/assets/" + geom->Attribute("mesh") + ".obj";
        sscanf(materialMap[geom->Attribute("material")].c_str(), "%f %f %f %f", 
               &color[0], &color[1], &color[2], &color[3]);
        meshes.push_back({meshPath, color});
    }

    links[bodyName] = cref<Link>(pos, quat, meshes);

    for (tinyxml2::XMLElement* childBody = body->FirstChildElement("body"); 
        childBody; childBody = childBody->NextSiblingElement("body")) 
    {
        graph[bodyName].push_back(childBody->Attribute("name"));
        parseBody(childBody, graph, links);
    }
}

void parseXML(const std::string &path,
              std::unordered_map<std::string, std::vector<std::string>> &graph,
              std::unordered_map<std::string, ref<Link>> &links) 
{
	tinyxml2::XMLDocument doc;

	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        ERROR("Error loading XML file: {0}", path);
        return;
    }

    baseDir = path.substr(0, path.find_last_of('/'));
    INFO("{0}", baseDir);

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
            materialMap[name] = rgba;
        }
    } else {ASSERT(false, "No <asset> element found!");}

    // recursive load
    for (tinyxml2::XMLElement* body = worldbody->FirstChildElement("body"); 
    	body; body = body->NextSiblingElement("body"))
    {
    	parseBody(body, graph, links);
    }
}

}