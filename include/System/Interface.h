#ifndef SKYNET_INTERFACE_H
#define SKYNET_INTERFACE_H

#include "Layer.h"

namespace Skynet
{
    
    class Interface : public Layer
    {
    public:
        Interface();
        ~Interface();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnRender() override;
        virtual void OnEvent(Event& event) override;
        virtual void OnUpdate(Timestep* ts) override;

        void Begin();
        void End();

    private:
        void Styling();

    private:
        float guiTime   = 0.0f;
        bool  dockspace = true;
    };

} // namespace Skynet


#endif // SKYNET_APPLICATION_H