#include "Guizmo.h"

namespace Skynet
{
    
    Guizmo::~Guizmo() {}

    void Guizmo::Update(ImDrawList* drawlist, float x, float y) 
    {
        this->config.drawlist = drawlist;
        this->config.x = x;
        this->config.y = y;
    }

    void Guizmo::DrawPositiveLine(const ImVec2 &center, const ImVec2 &axis, const ImU32 &color, const float radius, const float thickness, const char* text, const bool selected)
    {
        const auto lineEndPositive = ImVec2{ center.x + axis.x, center.y + axis.y };
        const auto textPosX = ImVec2{ static_cast<float>(floor(lineEndPositive.x - 3.0f)), static_cast<float>(floor(lineEndPositive.y - 6.0f)) };

        this->config.drawlist->AddLine(center, lineEndPositive, color, thickness);
        this->config.drawlist->AddCircleFilled(lineEndPositive, radius, color);

        if (selected) 
        {
            this->config.drawlist->AddCircle(lineEndPositive, radius, IM_COL32_WHITE, 0, 1.1f);
            this->config.drawlist->AddText(textPosX, IM_COL32_WHITE, text);
        }
        else this->config.drawlist->AddText(textPosX, IM_COL32_BLACK, text);
    }

    void Guizmo::DrawNegativeLine(const ImVec2 &center, const ImVec2 &axis, const ImU32 &color, const float radius, const bool selected) 
    {
        const auto end = ImVec2{ center.x - axis.x, center.y - axis.y };
        this->config.drawlist->AddCircleFilled(end, radius, color);
        if (selected) this->config.drawlist->AddCircle(end, radius, IM_COL32_WHITE, 0, 1.1f);        
    }

    void Guizmo::BuildViewMatrix(titan::mat4& view, ImVec3 const& pos, ImVec3 const& right, ImVec3 const& up, ImVec3 const& forward) 
    {
        view(0,0) = right[0];
        view(1,0) = right[1];
        view(2,0) = right[2];
        view(3,0) = (-pos[0] * right[0] + -pos[1] * right[1] + -pos[2] * right[2]);

        view(0,1) = up[0];
        view(1,1) = up[1];
        view(2,1) = up[2];
        view(3,1) = (-pos[0] * up[0] + -pos[1] * up[1] + -pos[2] * up[2]);

        view(0,2) = forward[0];
        view(1,2) = forward[1];
        view(2,2) = forward[2];
        view(3,2) = (-pos[0] * forward[0] + -pos[1] * forward[1] + -pos[2] * forward[2]);
        
        view(0,3) = 0;
        view(1,3) = 0;
        view(2,3) = 0;
        view(3,3) = 1;
    }

    bool Guizmo::Render(titan::mat4& view, titan::mat4& projection, const float distance) 
    {
        const float size = this->config.size;
        const float hSize = size * 0.5f;
        const auto center = ImVec2{this->config.x + hSize, this->config.y + hSize};

        titan::mat4 projview = projection * view;

        {
            projview(1,0) *= -1;
            projview(1,1) *= -1;
            projview(1,2) *= -1;
            projview(1,3) *= -1;
        }

        {
            float aspect = projection(1,1) / projection(0,0);
            projview(0,0) *= aspect; 
            projview(0,2) *= aspect;
        }
        
        const titan::vec4 x_axis = projview * titan::vec4(config.axisLengthScale * 18, 0, 0, 0);
        const titan::vec4 y_axis = projview * titan::vec4(0, config.axisLengthScale * 18, 0, 0);
        const titan::vec4 z_axis = projview * titan::vec4(0, 0, config.axisLengthScale * 18, 0);        

        const ImVec4 xAxis(x_axis[0], x_axis[1], x_axis[2], x_axis[3]);
        const ImVec4 yAxis(y_axis[0], y_axis[1], y_axis[2], y_axis[3]);
        const ImVec4 zAxis(z_axis[0], z_axis[1], z_axis[2], z_axis[3]);

        const bool interactive = distance > 0.0f;
        const ImVec2 mousePos = ImGui::GetIO().MousePos;

        const float hoverCircleRadius = hSize * config.hoverCircleRadiusScale;
        if (interactive && this->checkInsideCircle(center, hoverCircleRadius, mousePos)) 
            this->config.drawlist->AddCircleFilled(center, hoverCircleRadius, config.hoverCircleColor);

        const float positiveRadius = size * config.positiveRadiusScale;
        const float negativeRadius = size * config.negativeRadiusScale;
        const bool xPositiveCloser = 0.0f >= xAxis.w;
        const bool yPositiveCloser = 0.0f >= yAxis.w;
        const bool zPositiveCloser = 0.0f >= zAxis.w;

        // sort axis based on distance
        std::vector<std::pair<int, float>> pairs = { {0, xAxis.w}, {1, yAxis.w}, {2, zAxis.w}, {3, -xAxis.w}, {4, -yAxis.w}, {5, -zAxis.w} };
        sort(pairs.begin(), pairs.end(), [=](const std::pair<int, float>& aA, const std::pair<int, float>& aB) { return aA.second > aB.second; });

        // find selection, front to back
        int selection = -1;
        for (auto it = pairs.crbegin(); it != pairs.crend() && selection == -1 && interactive; ++it) 
        {
            switch (it->first) 
            {
            case 0: // +x axis
                if (this->checkInsideCircle(ImVec2{ center.x + xAxis.x, center.y + xAxis.y }, positiveRadius, mousePos)) selection = 0;
                break;
            case 1: // +y axis
                if (this->checkInsideCircle(ImVec2{ center.x + yAxis.x, center.y + yAxis.y }, positiveRadius, mousePos)) selection = 1;
                break;
            case 2: // +z axis
                if (this->checkInsideCircle(ImVec2{ center.x + zAxis.x, center.y + zAxis.y }, positiveRadius, mousePos)) selection = 2;
                break;
            case 3: // -x axis
                if (this->checkInsideCircle(ImVec2{ center.x - xAxis.x, center.y - xAxis.y }, negativeRadius, mousePos)) selection = 3;
                break;
            case 4: // -y axis
                if (this->checkInsideCircle(ImVec2{ center.x - yAxis.x, center.y - yAxis.y }, negativeRadius, mousePos)) selection = 4;
                break;
            case 5: // -z axis
                if (this->checkInsideCircle(ImVec2{ center.x - zAxis.x, center.y - zAxis.y }, negativeRadius, mousePos)) selection = 5;
                break;
            default: break;
            }
        }

        // draw back first
        const float lineThickness = size * config.lineThicknessScale;
        for (const auto& [fst, snd] : pairs) 
        {
            switch (fst) 
            {
                case 0: // +x axis
                    this->DrawPositiveLine(center, ImVec2{ xAxis.x, xAxis.y }, xPositiveCloser ? config.xCircleFrontColor : config.xCircleBackColor, positiveRadius, lineThickness, "X", selection == 0);
                    continue;
                case 1: // +y axis
                    this->DrawPositiveLine(center, ImVec2{ yAxis.x, yAxis.y }, yPositiveCloser ? config.yCircleFrontColor : config.yCircleBackColor, positiveRadius, lineThickness, "Y", selection == 1);
                    continue;
                case 2: // +z axis
                    this->DrawPositiveLine(center, ImVec2{ zAxis.x, zAxis.y }, zPositiveCloser ? config.zCircleFrontColor : config.zCircleBackColor, positiveRadius, lineThickness, "Z", selection == 2);
                    continue;
                case 3: // -x axis
                    this->DrawNegativeLine(center, ImVec2{ xAxis.x, xAxis.y }, !xPositiveCloser ? config.xCircleFrontColor : config.xCircleBackColor, negativeRadius, selection == 3);
                    continue;
                case 4: // -y axis
                    this->DrawNegativeLine(center, ImVec2{ yAxis.x, yAxis.y }, !yPositiveCloser ? config.yCircleFrontColor : config.yCircleBackColor, negativeRadius, selection == 4);
                    continue;
                case 5: // -z axis
                    this->DrawNegativeLine(center, ImVec2{ zAxis.x, zAxis.y }, !zPositiveCloser ? config.zCircleFrontColor : config.zCircleBackColor, negativeRadius, selection == 5);
                    continue;
                default: break;
            }
        }

        if (selection != -1 && ImGui::IsMouseClicked(ImGuiPopupFlags_MouseButtonLeft)) 
        {
            titan::mat4 model = view.inverse();

            const ImVec3 pivotPos = ImVec3{ &model(0,3) } + ImVec3{ &model(0,2) } * distance;

            if (selection == 0) this->BuildViewMatrix(view, pivotPos + ImVec3{ distance, 0, 0 }, ImVec3{ 0, 0, -1},  ImVec3{ 0, 1, 0 },  ImVec3{ 1, 0, 0 });  // +x axis
            if (selection == 1) this->BuildViewMatrix(view, pivotPos + ImVec3{ 0, distance, 0 }, ImVec3{ 1, 0, 0 },  ImVec3{ 0, 0, -1 }, ImVec3{ 0, 1, 0 });  // +y axis
            if (selection == 2) this->BuildViewMatrix(view, pivotPos + ImVec3{ 0, 0, distance }, ImVec3{ 1, 0, 0 },  ImVec3{ 0, 1, 0 },  ImVec3{ 0, 0, 1 });  // +z axis
            if (selection == 3) this->BuildViewMatrix(view, pivotPos - ImVec3{ distance, 0, 0 }, ImVec3{ 0, 0, 1 },  ImVec3{ 0, 1, 0 },  ImVec3{ -1, 0, 0 }); // -x axis
            if (selection == 4) this->BuildViewMatrix(view, pivotPos - ImVec3{ 0, distance, 0 }, ImVec3{ 1, 0, 0 },  ImVec3{ 0, 0, 1 },  ImVec3{ 0, -1, 0 }); // -y axis
            if (selection == 5) this->BuildViewMatrix(view, pivotPos - ImVec3{ 0, 0, distance }, ImVec3{ -1, 0, 0 }, ImVec3{ 0, 1, 0 },  ImVec3{ 0, 0, -1 }); // -z axis

            return true;
        }
        return false;
    }

} // namespace Skynet
