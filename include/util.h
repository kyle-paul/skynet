#ifndef SKYNET_UTIL_H
#define SKYNET_UTIL_H

#include "core.h"

namespace util {

template <typename T>
static bool renameEntity(std::unordered_map<std::string, ref<T>> &map, 
                         const std::string &oldName, const std::string &newName)
{
    if (oldName == newName) {
        return true;
    }

    if (map.find(newName) != map.end()) {
        ERROR("Rename failed: {0} already exists", newName);
        return false;
    }

    auto it = map.find(oldName);
    if (it == map.end()) {
        ERROR("Object name: {0} not found", oldName);
        return false;
    }

    map[newName] = std::move(it->second);
    map.erase(it);

    return true;
};

} // namespace util

#endif // SKYNET_UTIL_H