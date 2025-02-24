#include "ContentBrowser.h"
#include "imgui.h"

namespace Skynet
{
    static const std::filesystem::path assetPath = "../assets";

    ContentBrowser::ContentBrowser()
        : curdir(assetPath)
    {
        this->folderIcon = cref<Texture2D>("../assets/gui/icons/folder_icon.png"); 
        this->fileIcon = cref<Texture2D>("../assets/gui/icons/file_icon.png"); 
    }

    ContentBrowser::~ContentBrowser()
    {
        
    }

    void ContentBrowser::OnRender()
    {
        ImGui::Begin("Content Browser");

        if (curdir != std::filesystem::path(assetPath))
        {
            if (ImGui::Button("<-"))
            {
                curdir = curdir.parent_path();
            }
        }

        static float padding = 16.0f;
        static float thumbnail = 60.0f;

        float cellsize = thumbnail + padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellsize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (auto& directoryEntry : std::filesystem::directory_iterator(curdir))
        {
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, assetPath);

            std::string filename = relativePath.filename().string();
            ref<Texture2D> icon = directoryEntry.is_directory() ? folderIcon : fileIcon;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton(filename.c_str(), (ImTextureID)(uintptr_t)(icon->GetTextureID()), 
                               ImVec2(thumbnail, thumbnail), ImVec2(0, 1), ImVec2(1, 0));

            if (relativePath.string().find("mesh") != std::string::npos)
            {
                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Choose object..."))  { } 
					ImGui::EndPopup();
                }
                
                else if (ImGui::BeginDragDropSource())
                {
                    const char* itemPath = path.c_str();
                    ImGui::SetDragDropPayload("OBJECT_ITEM", itemPath, strlen(itemPath) + 1);
					ImGui::EndDragDropSource();
                }
            }

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory()) curdir /= path.filename();
			}

            ImGui::TextWrapped("%s", filename.c_str());
            ImGui::NextColumn();
        }

        ImGui::Columns(1);

        ImGui::SliderFloat("Thumbnail", &thumbnail, 16, 60);
        ImGui::SliderFloat("Padding", &padding, 0, 32);

        ImGui::End();
    }

} // namespace Skynet