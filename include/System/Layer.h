#ifndef SKYNET_LAYER_H
#define SKYNET_LAYER_H

#include "Timestep.h"
#include "Event.h"

namespace Skynet 
{

    class Layer 
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {};
        virtual void OnDetach() {};
        virtual void OnRender() {};
        virtual void OnUpdate(Timestep* ts) {};
        virtual void OnEvent(Event& event) {};

        inline const std::string& GetName() const {
            return name;
        }

    private:
        std::string name;
    };

} // namepsace Skynet

#endif // SKYNET_LAYER_H