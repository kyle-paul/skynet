#include "gui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "dialog.h"

static bool dockspace_open = true;

Interface::Interface(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplOpenGL3_Init("#version 410")) {
        ASSERT(false, "ImGui OpenGL3 Init failed!");
        return;
    }

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("../assets/gui/fonts/Arial.ttf", 14.0f);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    this->styling();

    iconPlay = cref<Texture2D>("../assets/gui/icons/play_button.png");
    iconEdit = cref<Texture2D>("../assets/gui/icons/stop_button.png");
}

Interface::~Interface() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Interface::styling() {
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.60f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowPadding     = ImVec2(8.00f, 8.00f);
    style.FramePadding      = ImVec2(5.00f, 2.00f);
    style.CellPadding       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing     = 25;
    style.ScrollbarSize     = 6;
    style.GrabMinSize       = 10;
    style.WindowBorderSize  = 1;
    style.ChildBorderSize   = 1;
    style.PopupBorderSize   = 1;
    style.FrameBorderSize   = 1;
    style.TabBorderSize     = 1;
    style.WindowRounding    = 7;
    style.ChildRounding     = 4;
    style.FrameRounding     = 3;
    style.PopupRounding     = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding      = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding       = 4;
}

void Interface::begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void Interface::end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Interface::menubar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open file", "Ctrl+O")) {
                IGFD::FileDialogConfig config; config.path = ".";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose file", ".xml", config);
                action = Action::OpenFile;
            }

            if (ImGui::MenuItem("Open scene", "Ctrl+L")) {
                IGFD::FileDialogConfig config; config.path = ".";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a directory", ".skynet", config);
                action = Action::OpenScene;
            }

            if (ImGui::MenuItem("Save scene", "Ctrl+S")) {
                IGFD::FileDialogConfig config; config.path = ".";
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a directory", ".skynet", config);
                action = Action::SaveScene;
            }

            if (ImGui::MenuItem("Clear scene", "Ctrl+L")) {
                scene->objects.clear();
                scene->links.clear();
                scene->joints.clear();
                scene->graph.clear();
                scene->selectedEntity = nullptr;
                scene->loaded = false;
            }

            if (ImGui::MenuItem("Exit")) {

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            
            ImGui::EndMenu();   
        }

        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("Tutorials");
            ImGui::MenuItem("Documentation");
            ImGui::MenuItem("About");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Open Dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string ext = path.substr(path.find_last_of('.') + 1);

            switch(this->action) {
                case(Action::OpenFile)  : this->scene->load(path);        break;
                case(Action::SaveScene) : this->serial.serialize(path);   break;
                case(Action::OpenScene) : this->serial.deserialize(path); break;
            }
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

void Interface::toolbar() {
    ref<Texture2D> icon = this->scene->state == SceneState::Edit ? iconPlay : iconEdit;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    // Get default color
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
    if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused()) ImGui::SetWindowCollapsed(false);
    else ImGui::SetWindowCollapsed(true);

	float size = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - size * 0.5f);

	if (ImGui::ImageButton("PlayButton", 
                           (ImTextureID)(uintptr_t)(icon->getTextureID()), 
                           ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1))) 
    {
		if (scene->state == SceneState::Edit) {
            scene->state = SceneState::Play;
		}
		else if (scene->state == SceneState::Play) {
            scene->state = SceneState::Edit;
        }
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void Interface::render(ref<Scene> &scene) {
    this->scene = scene; serial.setScene(scene);
    this->begin();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar  | ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove    | 
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace", &dockspace_open, window_flags);
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        this->menubar();
    }
    ImGui::PopStyleVar(2);
    ImGui::End();

    // toolbar
    this->toolbar();

    {
        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(scene->bgcol[0], scene->bgcol[1], scene->bgcol[2], scene->bgcol[3]);
        scene->render();
        framebuffer->unbind();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");

        this->viewport_hover = ImGui::IsWindowHovered() ? true : false;
        ImVec2 viewportPos = ImGui::GetCursorScreenPos();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (viewportSize.x > 0 && viewportSize.y > 0) {
            uint32_t newWidth = (uint32_t)viewportSize.x;
            uint32_t newHeight = (uint32_t)viewportSize.y;
            if (framebuffer->width != newWidth || framebuffer->height != newHeight) {
                framebuffer->resize(newWidth, newHeight);
                scene->camera.aspect = viewportSize.x / viewportSize.y;
            }
        }

        ImGui::Image(
            (ImTextureID)(intptr_t)framebuffer->color, 
            ImVec2(framebuffer->width, framebuffer->height), ImVec2(0, 1), ImVec2(1, 0));

        ImGuizmo::SetRect(viewportPos.x, viewportPos.y, viewportSize.x, viewportSize.y);
        ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        scene->editGuizmo();
        
        guizmo.config.drawlist = ImGui::GetWindowDrawList();
        guizmo.config.x = viewportPos.x + 15.0f;
        guizmo.config.y = viewportPos.y + viewportSize.y - 120.0f;
        guizmo.render(scene->camera.getProjView(), scene->camera.getProjection());

        ImGui::End();
        ImGui::PopStyleVar();
    }
    
    {
        ImGui::Begin("General setting");
        
        ImGui::Text("Environment");
        ImGui::DragFloat3("Light direction", scene->light, 0.2f, -50.0f, 50.0f);
        ImGui::DragFloat4("background color", scene->bgcol, 0.1f, 0.0f, 1.0f);

        ImGui::Text("Camera");
        ImGui::DragFloat("field of view", &scene->camera.fov, 1.0f, 10.0f, 90.0f);
        ImGui::DragFloat("near clip", &scene->camera.znear, 1.0f, -30.0f, 0.1f);
        ImGui::DragFloat("far clip", &scene->camera.zfar, 1.0f, 20.0f, 100.0f);
        ImGui::DragFloat3("position", scene->camera.p, 0.2f, -50.0f, 50.f);
        ImGui::DragFloat3("rotation", scene->camera.e, 0.1f, -45.0f, 45.0f);

        ImGui::Text("Option");
        ImGui::Checkbox("Use Guizmo", &scene->data->opt.guizmo_local);

        ImGui::End();
    }

    {
        ImGui::Begin("Engine experiment");

        if (ImGui::Button("inverse kinematics")) {
            this->scene->inverse();
        }

        if (ImGui::Button("reset joint")) {
            this->scene->reset();
        }
        

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        const char* current_camera = scene->cams.size() > 0 ? scene->cams[scene->currentCamera].c_str() : "null";
        
        if (ImGui::BeginCombo("Camera", current_camera)) {
            for (int n = 0; n < scene->cams.size(); n++) {
            bool is_selected = (scene->currentCamera == n);
            if (ImGui::Selectable(scene->cams[n].c_str(), is_selected)) {
                scene->currentCamera = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
            }
            ImGui::EndCombo();
        }
        ImGui::PopStyleColor(1);
        
        ImGui::End();
    }
    
    {
        ImGui::Begin("Objects setting");

        if (ImGui::BeginPopupContextWindow(0, 1)) {
            if (ImGui::BeginMenu("Create shape")) {
                if (ImGui::MenuItem("cube")) scene->create(Object::Cube, "cube");
                if (ImGui::MenuItem("sphere")) scene->create(Object::Sphere, "sphere");
                if (ImGui::MenuItem("capsule"))  scene->create(Object::Capsule, "capsule");
                if (ImGui::MenuItem("tetrahedra")) scene->create(Object::Tetrahedra, "tetrahedra");
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Create camera")) {
                scene->create(Object::Camera, "runtime_camera_1");
            }

            if (ImGui::MenuItem("Create light")) {

            }
            ImGui::EndPopup();
        }

        for (auto &[name, link] : scene->links) {
            if (ImGui::TreeNode(name.c_str())) {
                ImGui::DragFloat3("position", link->p_w, 0.01f, -10.0f, 10.0f);
                ImGui::DragFloat4("quaternion", link->q, 0.01f, -10.0f, 10.0f);
                ImGui::DragFloat3("scale", link->s, 0.01f, 0.0f, 10.0f);
                ImGui::DragFloat3("com", link->com_w, 0.01f, 0.0f, 10.0f);
                ImGui::DragFloat("mass", &link->mass, 0.01f, 0.0f, 10.0f);
                ImGui::TreePop();
            }
        }

        for (auto &[name, joint] : scene->joints) {
            if (ImGui::TreeNode(name.c_str())) {
                ImGui::DragFloat3("axis", joint->w_w, 0.01f, -2.0f, 2.0f);
                ImGui::DragFloat3("anchor", joint->p_w, 0.01f, -10.0f, 10.0f);
                ImGui::DragFloat("angle", &joint->a, 0.1f, -2*PI, 2*PI);
                ImGui::TreePop();
            }
        }

        std::string old_name, new_name;

        for (auto &[name, object] : scene->objects) {
            if (ImGui::TreeNode(name.c_str())) {
                scene->selectedEntity = object;
                static char newName[128];
                strcpy(newName, name.c_str());

                if (ImGui::InputText("Name", newName, IM_ARRAYSIZE(newName), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    old_name = name; new_name = newName;
                }
                ImGui::ColorEdit4("color", object->color);
                ImGui::DragFloat3("position", object->p, 0.01f, -2.0f, 2.0f);
                ImGui::DragFloat3("euler", object->e, 0.01f, -2.0f, 2.0f);
                ImGui::DragFloat3("scale", object->s, 0.01f, 0.0f, 10.0f);
                ImGui::DragFloat3("velocity", object->vel, 0.01f, -10.0f, 10.0f);
                ImGui::DragFloat3("acceleration", object->acc, 0.01f, -10.0f, 10.0f);
                ImGui::TreePop();
            }
        }

        if (!old_name.empty() && old_name != new_name) {
            util::renameEntity(scene->objects, old_name, new_name);
        }

        ImGui::End();
    }
    this->end();
}