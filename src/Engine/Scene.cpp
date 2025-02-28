#include "Scene.h"
#include "Math.h"
#include "Input.h"
#include "Component.h"
#include "Renderer.h"
#include "ImGuizmo.h"
#include "GenEntity.h"
#include "Contact.h"
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
        boxshad = cref<Shader>("box", "../assets/glsl/bvh.glsl");

        /* Default ground */
        entt::entity ground = bodies.create();
        bodies.emplace<MeshComp>(ground, Object::Cube, 3.0f, 0.01f, 3.0f);
        bodies.emplace<TagComp>(ground, "ground");
        bodies.emplace<TextureComp>(ground);
        bodies.emplace<RigidBodyComp>(ground, BodyType::Static, 100.0f);
        bodies.emplace<BVHComp>(ground, bodies.get<MeshComp>(ground).mesh);
        bodies.get<MeshComp>(ground).mesh->InitGL();
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

        /* Drag and drop browser content */ 
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *object_payload = ImGui::AcceptDragDropPayload("OBJECT_ITEM");
        
            if (object_payload != nullptr && object_payload->Data != nullptr)
            {
                std::string path(static_cast<const char*>(object_payload->Data), object_payload->DataSize - 1);
                Entity::GenerateEntity(bodies, path.substr(path.find_last_of("/\\") + 1), path);
            }

            ImGui::EndDragDropTarget();
        }
        
        ImGui::End();
        ImGui::PopStyleVar();
    };

    void Scene::OnUpdate(Timestep* ts)
    {
        this->UpdateVisualize();
        
        /* Check contact */
        // if (bodies.size() == 3)
        // {
        //     auto& boxA  = bodies.get<BVHComp>((entt::entity)1).node->box;
        //     auto& bodyA = bodies.get<RigidBodyComp>((entt::entity)1).body;

        //     titan::vec3 minA = boxA.GetTransMin(bodyA.GetTransform());
        //     titan::vec3 maxA = boxA.GetTransMax(bodyA.GetTransform());

        //     auto& boxB  = bodies.get<BVHComp>((entt::entity)2).node->box;
        //     auto& bodyB = bodies.get<RigidBodyComp>((entt::entity)2).body;

        //     titan::vec3 minB = boxA.GetTransMin(bodyB.GetTransform());
        //     titan::vec3 maxB = boxA.GetTransMax(bodyB.GetTransform());

        //     if(BVH::IsCollision(minA, maxA, minB, maxB))
        //     {
        //         INFO("Collision");
        //         Contact::ComputeContact(boxA, boxB, bodyA, bodyB);
        //     }
        // }

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
            shader->SetMat4("projview", camera->GetProjView().raw());
            shader->SetFloat3("viewPos", camera->p.raw());
            shader->SetFloat3("light", light);

            auto view = bodies.view<RigidBodyComp, MeshComp, BVHComp, TextureComp>();
            for (auto entity : view)
            {
                auto& rigid_comp   = view.get<RigidBodyComp>(entity);
                auto& mesh_comp    = view.get<MeshComp>(entity);
                auto& texture_comp = view.get<TextureComp>(entity);
                auto& bvh_comp     = view.get<BVHComp>(entity);

                rigid_comp.body.UpdateTransform();

                shader->Bind();
                shader->SetFloat4("color", texture_comp.color);
                shader->SetMat4("model", rigid_comp.body.GetTransform().raw());
                Renderer::Draw(mesh_comp.mesh->GetVA());
                shader->Unbind();

                boxshad->Bind();
                boxshad->SetMat4("projview", camera->GetProjView().raw());
                boxshad->SetMat4("transform", rigid_comp.body.GetTransform().raw());
                bvh_comp.Render(boxshad);
                boxshad->Unbind();
            }

            shader->Unbind();
        }

        {
            vecshad->Bind();
            vecshad->SetMat4("projview", camera->GetProjView().raw());

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
        /* Simulation integration */
        ODE::EulerStep(bodies, 0, ts->GetSeconds());

        /* Check collision */
        BVH::CheckCollision(bodies, this->k, this->b);
    }

    void Scene::EditGuizmo()
    {
        if (selectedEntityID == entt::null) 
            return;

        auto& c = bodies.get<RigidBodyComp>(selectedEntityID);
        titan::mat4 T = c.body.GetTransform();

        ImGuizmo::Manipulate(camera->GetView().raw(), camera->GetProjection().raw(), 
                            (ImGuizmo::OPERATION)typeGuizmo, ImGuizmo::LOCAL, T.raw());
        
        if (state == SceneState::Edit && ImGuizmo::IsUsing) {
            titan::Decompose(T, c.body.x, c.body.s, c.body.e);
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
