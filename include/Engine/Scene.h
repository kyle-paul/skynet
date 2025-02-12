#ifndef SKYNET_SCENE_H
#define SKYNET_SCENE_H

#include "System.h"
#include "Data.h"
#include "Layer.h"
#include "Camera.h"
#include "Shader.h"
#include "Frame.h"
#include "Guizmo.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "entt.hpp"

namespace Skynet
{
    
    class Scene : public Layer
    {
    public:
        Scene();
        ~Scene();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnRender() override;
        virtual void OnUpdate(Timestep* ts) override;
        virtual void OnEvent(Event& event) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnMouseMoveEvent(MouseMovedEvent& event);
        bool OnMouseScrollEvent(MouseScrolledEvent& event);
        bool OnMouseReleasedEvent(MouseButtonReleasedEvent& event);

        void UpdatePhysics(Timestep* ts);
        void UpdateVisualize();
        void EditGuizmo();

    private:

        float lastx, lasty;
        bool  firstLeft, firstRight = true;
        
        uint32_t typeGuizmo = 0;
        SceneState state = SceneState::Edit;

        ref<Camera> camera;
        ref<Shader> shader;
        ref<Shader> vecshad;
        ref<Guizmo> guizmo;
        ref<Frame> frame;

        entt::entity selectedEntityID = entt::null;
        entt::registry bodies;
        entt::registry vectors;

        float y[1 * STATE_SIZE];
        float ydot[1 * STATE_SIZE];

        float k = 16.0f;
        float b = 9.0f;

        float light[3] = { -10.0f, -10.0f, 10.0f };
        float background[4] = {0.15f, 0.15f, 0.15f, 1.0f};

        friend class Global;
        friend class Serializer;
    };

} // namespace Skynet


#endif // SKYNET_SCENE_H