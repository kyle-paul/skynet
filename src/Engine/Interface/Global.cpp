#include "Global.h"
#include "Component.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "FileDialog.h"
#include "GenEntity.h"

namespace Skynet
{
    static void DrawVec3Control(const std::string& label, float* values, float reset = 0.0f, float speed = 0.1f, float width = 120.0f)
    {
        ImGuiIO &io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[1];

        ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, width);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) values[0] = reset;
        ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values[0], speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) values[1] = reset;
        ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values[1], speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) values[2] = reset;
        ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values[2], speed, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
    }


    Global::Global(Scene* scene) : scene(scene)
    {
        serializer.SetScene(scene);

        iconPlay = cref<Texture2D>("../assets/gui/icons/play_button.png");
        iconEdit = cref<Texture2D>("../assets/gui/icons/stop_button.png");
    }

    Global::~Global()
    {
        
    }

    void Global::OnRender()
    {        
        this->RenderMenubar();
        this->RenderEnvironment();
        this->RenderObjectSetting();
        this->RenderToolBar();
        this->RenderExperiment();
        this->browser.OnRender();
    }

    void Global::RenderMenubar()
    {
        if (ImGui::BeginMenuBar()) 
        {
            if (ImGui::BeginMenu("File")) 
            {
                if (ImGui::MenuItem("Open file", "Ctrl+O")) 
                {
                    IGFD::FileDialogConfig config; config.path = ".";
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose file", ".xml", config);
                    action = Action::OpenFile;
                }

                if (ImGui::MenuItem("Open scene", "Ctrl+L")) 
                {
                    IGFD::FileDialogConfig config; config.path = ".";
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a directory", ".skynet", config);
                    action = Action::OpenScene;
                }

                if (ImGui::MenuItem("Save scene", "Ctrl+S")) 
                {
                    IGFD::FileDialogConfig config; config.path = ".";
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a directory", ".skynet", config);
                    action = Action::SaveScene;
                }

                if (ImGui::MenuItem("Clear scene", "Ctrl+L")) 
                {

                }

                if (ImGui::MenuItem("Exit")) 
                {

                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) 
            {    
                ImGui::EndMenu();   
            }

            if (ImGui::BeginMenu("Help")) 
            {
                ImGui::MenuItem("Tutorials");
                ImGui::MenuItem("Documentation");
                ImGui::MenuItem("About");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Open Dialog
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) 
        {
            if (ImGuiFileDialog::Instance()->IsOk()) 
            {
                std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string ext = path.substr(path.find_last_of('.') + 1);

                switch(this->action) {
                    case(Action::SaveScene) : this->serializer.Serialize(path); break;
                    case(Action::OpenScene) : this->serializer.Deserialize(path); break;
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }

    }

    void Global::RenderEnvironment()
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Begin("Setting");

        ImGui::PushFont(io.Fonts->Fonts[1]);
        ImGui::Text("Camera control");
        ImGui::PopFont();
        DrawVec3Control("position", scene->camera->p.raw());
        DrawVec3Control("rotation", scene->camera->e.raw());
        ImGui::Separator();

        ImGui::PushFont(io.Fonts->Fonts[1]);
        ImGui::Text("Spring-damper system");
        ImGui::PopFont();
        ImGui::DragFloat("Spring coef", &scene->k);
        ImGui::DragFloat("Damping coef", &scene->b);
        ImGui::Separator();
        
        ImGui::End();
    }

    void Global::RenderObjectSetting()
    {
        /* Object hierarchy panel */
        {
            ImGui::Begin("Object Hierarchy");

            scene->bodies.each([&](auto entityID)
            {
                this->DrawEntityNode(entityID);
            });

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) 
            {
                scene->selectedEntityID = entt::null;
            }

            if (ImGui::BeginPopupContextWindow(0, 1)) 
            {
                if (ImGui::BeginMenu("Create shape")) 
                {
                    if (ImGui::MenuItem("cube")) 
                    {
                        entt::entity body = scene->bodies.create();
                        scene->bodies.emplace<MeshComp>(body, Object::Cube, 0.5f, 0.5f, 0.5f);
                        scene->bodies.emplace<TagComp>(body, "cube");
                        scene->bodies.emplace<TextureComp>(body);
                        scene->bodies.emplace<RigidBodyComp>(body, BodyType::Dynamic, 0.5f);
                        scene->bodies.emplace<BVHComp>(body, scene->bodies.get<MeshComp>(body).mesh);
                        scene->bodies.get<MeshComp>(body).mesh->InitGL();
                    }

                    if (ImGui::MenuItem("sphere")) 
                    {
                        entt::entity body = scene->bodies.create();
                        scene->bodies.emplace<MeshComp>(body, Object::Sphere, 0.5f);
                        scene->bodies.emplace<TagComp>(body, "sphere");
                        scene->bodies.emplace<TextureComp>(body);
                        scene->bodies.emplace<RigidBodyComp>(body, BodyType::Dynamic, 0.5f);
                        scene->bodies.emplace<BVHComp>(body, scene->bodies.get<MeshComp>(body).mesh);
                        scene->bodies.get<MeshComp>(body).mesh->InitGL();
                    }

                    if (ImGui::MenuItem("capsule")) { }
                    if (ImGui::MenuItem("tetrahedra")) { }
                    
                    ImGui::EndMenu();
                }

                if (ImGui::MenuItem("Create camera")) 
                {

                }

                if (ImGui::MenuItem("Create light")) 
                {

                }
                ImGui::EndPopup();
            }

            ImGui::End();
        }


        /* Properties panel */
        {
            ImGui::Begin("Properties");

            if (scene->selectedEntityID != entt::null)
            {
                this->DisplayProperties(scene->selectedEntityID);
            }

            ImGui::End();
        }
    }

    void Global::DrawEntityNode(entt::entity entityID)
    {
        auto& tc = scene->bodies.get<TagComp>(entityID);
        ImGui::Text("%s", tc.tag.c_str());

        ImGuiTreeNodeFlags flags = (scene->selectedEntityID == entityID) ? ImGuiTreeNodeFlags_Selected : 0;
        flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entityID, flags, "%s", tc.tag.c_str());

        if (ImGui::IsItemClicked()) scene->selectedEntityID = entityID;

        if (opened)
        {
            ImGui::TreePop();
        } 
    }

    void Global::DisplayProperties(entt::entity entityID)
    {
        ImGuiIO &io = ImGui::GetIO();
    
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(9.0f, 9.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(9.0f, 9.0f));
        
        if (scene->bodies.has<TagComp>(entityID))
        {
            auto& c = scene->bodies.get<TagComp>(entityID);
            char buffer[256];

            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, c.tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                c.tag = std::string(buffer);
            }

            ImGui::Separator();
        }

        if (scene->bodies.has<TextureComp>(entityID))
        {
            ImGui::PushFont(io.Fonts->Fonts[1]);
            ImGui::Text("Texture component");
            ImGui::PopFont();

            auto& c = scene->bodies.get<TextureComp>(entityID);
            ImGui::ColorEdit4("Color", c.color);

            ImGui::Separator();
        }

        if (scene->bodies.has<RigidBodyComp>(entityID))
        {
            ImGui::PushFont(io.Fonts->Fonts[1]);
            ImGui::Text("Rigidbody component");
            ImGui::PopFont();

            auto& c = scene->bodies.get<RigidBodyComp>(entityID);

            const char* options[] = { "Static", "Dynamic" };
            const char* curString = BodyTypeToString(c.type).c_str();

            if (ImGui::BeginCombo("Body Type", curString))
            {
                for (int n = 0; n < IM_ARRAYSIZE(options); n++)
                {
                    bool isSelected = (curString == options[n]);
                    if (ImGui::Selectable(options[n], isSelected))
                    {
                        curString = options[n];
                        c.type = StringToBodyType(options[n]);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            DrawVec3Control("Position", c.body.x.raw());
            DrawVec3Control("Rotation", c.body.e.raw());
            DrawVec3Control("Scale", c.body.s.raw());
            ImGui::Separator();
        }

        if (scene->bodies.has<BVHComp>(entityID))
        {
            ImGui::PushFont(io.Fonts->Fonts[1]);
            ImGui::Text("Collision component");
            ImGui::PopFont();

            auto& bvh_comp = scene->bodies.get<BVHComp>(entityID);

            ImGui::Text("Tree depth");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.0f);
            ImGui::InputInt("##TreeDepth", &bvh_comp.maxDepth);

            ImGui::SameLine();
            if (ImGui::Button("Compute BVH"))
            {
                bvh_comp.compute(scene->bodies.get<MeshComp>(entityID).mesh);
            }

            ImGui::ColorEdit4("BVH Color", bvh_comp.color.raw());
            ImGui::Separator();
        }


        ImGui::PopStyleVar(2);
    }

    void Global::RenderToolBar()
    {
        ref<Texture2D> icon = scene->state == SceneState::Edit ? iconPlay : iconEdit;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        // Get default color
        auto& colors = ImGui::GetStyle().Colors;
        const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

        const auto& buttonActive = colors[ImGuiCol_ButtonActive];
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | 
                                           ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
        
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused()) ImGui::SetWindowCollapsed(false);
        else ImGui::SetWindowCollapsed(true);

        float size = ImGui::GetWindowHeight() - 4.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - size * 0.5f);

        if (ImGui::ImageButton("PlayButton", 
                              (ImTextureID)(uintptr_t)(icon->GetTextureID()), 
                              ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1))) 
        {
            if (scene->state == SceneState::Edit) 
            {
                scene->state = SceneState::Play;
            }
            else if (scene->state == SceneState::Play) 
            {
                scene->state = SceneState::Edit;
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    void Global::RenderExperiment()
    {
        ImGui::Begin("Scenario Experiemnt");

        ImGui::End();
    }   

} // namepsace Skynets