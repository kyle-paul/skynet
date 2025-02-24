#ifndef SKYNET_INTERFACE_GLOBAL_H
#define SKYNET_INTERFACE_GLOBAL_H

#include "Layer.h"
#include "Scene.h"
#include "Data.h"
#include "Texture.h"
#include "Serializer.h"

namespace Skynet
{
    
    class Global : public Layer
    {
    public:
        Global(Scene* scene);
        ~Global();

        virtual void OnRender() override;

    private:
        void RenderMenubar();
        void RenderEnvironment();
        void RenderObjectSetting();
        void RenderToolBar();
        void RenderExperiment();
        
        void DrawEntityNode(entt::entity entityID);
        void DisplayProperties(entt::entity entityID);

    private:
        Scene* scene;
        Action action;

        Serializer serializer;

        ref<Texture2D> iconPlay;
        ref<Texture2D> iconEdit;

        float dir[3]   = {0.2f, 0.4f, -0.3f};
        float pos[3]   = {0.5f, 0.5f, 0.5f};
        float force[3];
        
        bool dockspace = true;
    };

} // namespace Skynet


#endif // SKYNET_INTERFACE_GLOBAL_H