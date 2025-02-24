#ifndef SKYNET_INTERFACE_GLOBAL_H
#define SKYNET_INTERFACE_GLOBAL_H

#include "Layer.h"
#include "Scene.h"
#include "Data.h"
#include "Texture.h"
#include "Serializer.h"
#include "ContentBrowser.h"

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

        ContentBrowser browser;

        bool dockspace = true;
    };

} // namespace Skynet


#endif // SKYNET_INTERFACE_GLOBAL_H