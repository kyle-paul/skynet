#ifndef SKYNET_ENGINE_ENTITY_GENENTITY_H
#define SKYNET_ENGINE_ENTITY_GENENTITY_H

#include "System.h"
#include "entt.hpp"

namespace Skynet
{
    namespace Entity
    {
        void GenerateEntity(entt::registry& bodies, const std::string& name, const std::string& path);
        
    } // namepsace Entity

} // namespace Skynet


#endif // SKYNET_ENGINE_ENTITY_GENENTITY_H