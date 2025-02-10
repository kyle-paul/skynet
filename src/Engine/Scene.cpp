#include "Scene.h"
#include "Math.h"
#include "Input.h"
#include "Component.h"
#include "Renderer.h"
#include "ImGuizmo.h"
#include "ODE.h"

namespace Skynet
{

    Scene::Scene() 
    {
        
    }
    
    Scene::~Scene() 
    {

    }

    void Scene::OnAttach()
    {
        frame = cref<Frame>();
        camera = cref<Camera>();
        guizmo = cref<Guizmo>();
        shader = cref<Shader>("scene", "../assets/glsl/scene.glsl");
        vecshad = cref<Shader>("vector", "../assets/glsl/vector.glsl");
    };

    void Scene::OnDetach()
    {

    };

    void Scene::OnRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");

        /* Resize viewport */
        ImVec2 viewportPos = ImGui::GetCursorScreenPos();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x > 0 && viewportSize.y > 0) 
        {
            uint32_t width = (uint32_t)viewportSize.x;
            uint32_t height = (uint32_t)viewportSize.y;

            if (frame->GetWidth() != width || frame->GetHeight() != height) 
            {
                frame->Resize(width, height);
                camera->aspect = viewportSize.x / viewportSize.y;
            }
        }

        /* Render viewport on texture */
        ImGui::Image((ImTextureID)(intptr_t)frame->GetColor(), 
        ImVec2(frame->GetWidth(), frame->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

        /* Global guizmo */
        guizmo->Update(ImGui::GetWindowDrawList(), viewportPos.x + 15, viewportPos.y + viewportSize.y - 120);
        guizmo->Render(camera->GetProjView(), camera->GetProjection());

        /* Local guizmo */ 
        ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);
        ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        this->EditGuizmo();
        
        ImGui::End();
        ImGui::PopStyleVar();
    };

    void Scene::OnUpdate(Timestep* ts)
    {
        this->UpdateVisualize();
        if (this->state == SceneState::Play) 
            this->UpdatePhysics(ts);
    };

    void Scene::UpdateVisualize()
    {
        camera->UpdateProjView();

        frame->Bind();
            
        Renderer::SetClearColor(background);
        Renderer::ClearBufferBit();

        {
            shader->Bind();
            shader->SetMat4("projview", camera->GetProjView());
            shader->SetFloat3("viewPos", camera->p);
            shader->SetFloat3("light", light);

            auto view = bodies.view<RigidBodyComp, MeshComp, TextureComp>();
            for (auto entity : view)
            {
                auto& rigid_comp   = view.get<RigidBodyComp>(entity);
                auto& mesh_comp    = view.get<MeshComp>(entity);
                auto& texture_comp = view.get<TextureComp>(entity);

                shader->SetFloat4("color", texture_comp.color);
                shader->SetMat4("model", rigid_comp.body.GetTransform());
                Renderer::Draw(mesh_comp.mesh->GetVA());
            }

            shader->Unbind();
        }

        {
            vecshad->Bind();
            vecshad->SetMat4("projview", camera->GetProjView());

            auto view = vectors.view<VectorComp>();
            for (auto entity : view)
            {
                auto& vector_comp = view.get<VectorComp>(entity);
                Renderer::DrawLine(vector_comp.vector.GetVA());
            }

            vecshad->Unbind();
        }

        frame->Unbind();
    }

    void Scene::UpdatePhysics(Timestep* ts)
    {
        ODE::EulerStep(bodies, 0, ts->GetSeconds(), this->y, this->ydot);
    }

    void Scene::EditGuizmo()
    {
        if (selectedEntityID == entt::null) 
            return;

        auto& c = bodies.get<RigidBodyComp>(selectedEntityID);
        float* T = c.body.GetTransform();

        ImGuizmo::Manipulate(camera->GetView(), camera->GetProjection(), 
                 (ImGuizmo::OPERATION)typeGuizmo, ImGuizmo::LOCAL, T);
        
        if (state == SceneState::Edit && ImGuizmo::IsUsing) {
            Math::Decompose(T, c.body.x, c.body.s, c.body.omega);
        }
    }

    void Scene::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(Scene::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseMoveEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseScrollEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNCTION(Scene::OnMouseReleasedEvent));
    };

    bool Scene::OnKeyPressed(KeyPressedEvent& event)
    {
        if (event.IsRepeat()) return false;

        switch(event.GetKeyCode())
        {
            case(Key::T) : typeGuizmo = ImGuizmo::OPERATION::TRANSLATE; break;
            case(Key::R) : typeGuizmo = ImGuizmo::OPERATION::ROTATE; break;
            case(Key::S) : typeGuizmo = ImGuizmo::OPERATION::SCALE; break;
            case(Key::Q) : typeGuizmo = 0; break;
        }

        return true;
    }

    bool Scene::OnMouseMoveEvent(MouseMovedEvent& event)
    {
        if (Input::IsKeyPressed(Key::LeftShift) && Input::IsMouseButtonPressed(Mouse::ButtonLeft))
        {
            auto [xpos, ypos] = Input::GetMousePos();

            if (firstLeft)
            {
                lastx = xpos;
                lasty = ypos;

                firstLeft = false;
            }
            
            camera->e[1] += 0.01f * (xpos - lastx);
            camera->e[0] += 0.01f * (ypos - lasty);

            lastx = xpos;
            lasty = ypos;
        }

        else if (Input::IsKeyPressed(Key::LeftShift) && Input::IsMouseButtonPressed(Mouse::ButtonRight))
        {
            auto [xpos, ypos] = Input::GetMousePos();

            if (firstRight)
            {
                lastx = xpos;
                lasty = ypos;

                firstRight = false;
            }

            camera->p[0] -= 0.01f * (xpos - lastx);
            camera->p[1] += 0.01f * (ypos - lasty);

            lastx = xpos;
            lasty = ypos;
        }

        return true;
    }

    bool Scene::OnMouseScrollEvent(MouseScrolledEvent& event)
    {
        camera->p[2] += -0.1f * event.GetYOffset();
        return true;
    }

    bool Scene::OnMouseReleasedEvent(MouseButtonReleasedEvent& event)
    {
        firstLeft = true;
        firstRight = true;
        return true;
    }

} // namespace Skynet
