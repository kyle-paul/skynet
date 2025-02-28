#include "GenEntity.h"
#include "Component.h"

namespace Skynet
{
    namespace Entity
    {
        void GenerateEntity(entt::registry& bodies, const std::string& name, const std::string& path)
        {
            entt::entity body = bodies.create();
            bodies.emplace<MeshComp>(body, Object::Mesh, path, Loader::Assimp);
            bodies.emplace<TagComp>(body, name);
            bodies.emplace<TextureComp>(body);
            bodies.emplace<RigidBodyComp>(body, BodyType::Dynamic, 0.5f);
            bodies.emplace<BVHComp>(body, bodies.get<MeshComp>(body).mesh);
            bodies.get<MeshComp>(body).mesh->InitGL();
            
        } // namespace Entity

    } // namespace Skynet
    

} // namespace Skynet
