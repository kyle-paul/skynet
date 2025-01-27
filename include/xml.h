#pragma once

#include "core.h"
#include "link.h"
#include <tinyxml2.h>

namespace xml {

void parseBody(tinyxml2::XMLElement* body,
               std::unordered_map<std::string, std::vector<std::string>> &graph,
               std::unordered_map<std::string, ref<Link>> &links);

void parseXML(const std::string &path, 
              std::unordered_map<std::string, std::vector<std::string>> &graph,
              std::unordered_map<std::string, ref<Link>> &links);

} // namespace xml