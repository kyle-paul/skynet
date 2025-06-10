#ifndef SKYNET_ENGINE_SERIALIZE_H
#define SKYNET_ENGINE_SERIALIZE_H

#include "Scene.h"
#include "yaml-cpp/yaml.h"

namespace Skynet
{
    class Serializer
    {
    public:
        Serializer();
        ~Serializer();

        inline void SetScene(Scene* scene) { this->scene=scene; }
        
        void Serialize(const std::string& path);
        bool Deserialize(const std::string& path);

    private:	
        Scene* scene;
    };

} // namespace Skynet


#endif // SKYNET_ENGINE_SERIALIZE_H