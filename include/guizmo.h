#include "core.h"
#include "imgui.h"
#include "math.h"

struct Config {
    float x = 0.0f;
    float y = 0.0f;
    float size = 100.f;
    ImDrawList* drawlist = nullptr;

    // should be default
    float lineThicknessScale = 0.017f;
    float axisLengthScale = 0.33f;
    float positiveRadiusScale = 0.075f;
    float negativeRadiusScale = 0.05f;
    float hoverCircleRadiusScale = 0.88f;
    ImU32 xCircleFrontColor = IM_COL32(255, 54, 83, 255);
    ImU32 xCircleBackColor = IM_COL32(154, 57, 71, 255);
    ImU32 yCircleFrontColor = IM_COL32(138, 219, 0, 255);
    ImU32 yCircleBackColor = IM_COL32(98, 138, 34, 255);
    ImU32 zCircleFrontColor = IM_COL32(44, 143, 255, 255);
    ImU32 zCircleBackColor = IM_COL32(52, 100, 154, 255);
    ImU32 hoverCircleColor = IM_COL32(100, 100, 100, 130);
};

struct ImVec3 {
    ImVec3(const float x, const float y, const float z) : vec{ x, y, z } {}
    explicit ImVec3(const float* const data) : vec{ data[0], data[1], data[2] } {}
    float operator[](const int idx) const { return vec[idx]; }
    ImVec3 operator+(const ImVec3& other) const { return { vec[0] + other[0], vec[1] + other[1], vec[2] + other[2] }; }
    ImVec3 operator-(const ImVec3& other) const { return { vec[0] - other[0], vec[1] - other[1], vec[2] - other[2] }; }
    ImVec3 operator*(const float scalar)  const { return { vec[0] * scalar,   vec[1] * scalar,   vec[2] * scalar   }; }
    ImVec3 operator*(const ImVec3& other) const { return { vec[0] * other[0], vec[1] * other[1], vec[2] * other[2] }; }
    float vec[3];
};

class Guizmo
{
public:
    Guizmo() = default;
    Guizmo(ImDrawList* drawlist);
    ~Guizmo();

private:
    inline bool checkInsideCircle(const ImVec2 center, const float radius, const ImVec2 point)  {
        return (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y) <= radius * radius;
    }

    void drawPositiveLine(const ImVec2 &center, const ImVec2 &axis, const ImU32 &color, const float radius, const float thickness, const char* text, const bool selected);
    void drawNegativeLine(const ImVec2 &center, const ImVec2 &axis, const ImU32 &color, const float radius, const bool selected);
    void buildViewMatrix(float* view, ImVec3 const &pos, ImVec3 const &right, ImVec3 const &up, ImVec3 const &forward);
    bool render(float* view, float* projection, const float distance = 0.0f);

private:
    Config config;
	friend class Interface;
};