#pragma once

#ifndef IMGUIZMO_NAMESPACE
#define IMGUIZMO_NAMESPACE ImGuizmo
#endif

struct ImGuiWindow;

namespace IMGUIZMO_NAMESPACE
{
   IMGUI_API void SetDrawlist(ImDrawList* drawlist = nullptr);
   IMGUI_API void BeginFrame();
   IMGUI_API void SetImGuiContext(ImGuiContext* ctx);
   IMGUI_API bool IsOver();
   IMGUI_API bool IsUsing();
   IMGUI_API bool IsUsingViewManipulate();
   IMGUI_API bool IsViewManipulateHovered();
   IMGUI_API bool IsUsingAny();
   IMGUI_API void Enable(bool enable);

   IMGUI_API void SetRect(float x, float y, float width, float height);
   IMGUI_API void SetOrthographic(bool isOrthographic);

   enum OPERATION
   {
      TRANSLATE_X      = (1u << 0),
      TRANSLATE_Y      = (1u << 1),
      TRANSLATE_Z      = (1u << 2),
      ROTATE_X         = (1u << 3),
      ROTATE_Y         = (1u << 4),
      ROTATE_Z         = (1u << 5),
      ROTATE_SCREEN    = (1u << 6),
      SCALE_X          = (1u << 7),
      SCALE_Y          = (1u << 8),
      SCALE_Z          = (1u << 9),
      BOUNDS           = (1u << 10),
      SCALE_XU         = (1u << 11),
      SCALE_YU         = (1u << 12),
      SCALE_ZU         = (1u << 13),

      TRANSLATE = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,
      ROTATE    = ROTATE_X    | ROTATE_Y    | ROTATE_Z    | ROTATE_SCREEN,
      SCALE     = SCALE_X     | SCALE_Y     | SCALE_Z,
      SCALEU    = SCALE_XU    | SCALE_YU    | SCALE_ZU,
      UNIVERSAL = TRANSLATE   | ROTATE      | SCALEU
   };

   inline OPERATION operator|(OPERATION lhs, OPERATION rhs)
   {
     return static_cast<OPERATION>(static_cast<int>(lhs) | static_cast<int>(rhs));
   }

   enum MODE
   {
      LOCAL,
      WORLD
   };

   IMGUI_API bool Manipulate(const float* view, const float* projection, OPERATION operation, MODE mode, float* matrix, float* deltaMatrix = NULL, const float* snap = NULL, const float* localBounds = NULL, const float* boundsSnap = NULL);
   IMGUI_API void ViewManipulate(float* view, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);
   IMGUI_API void ViewManipulate(float* view, const float* projection, OPERATION operation, MODE mode, float* matrix, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);
   IMGUI_API void SetAlternativeWindow(ImGuiWindow* window);

   [[deprecated("Use PushID/PopID instead.")]]
   IMGUI_API void SetID(int id);

	IMGUI_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
	IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
	IMGUI_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
	IMGUI_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
	IMGUI_API void          PopID();                                                        // pop from the ID stack.
	IMGUI_API ImGuiID       GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter).
	IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
	IMGUI_API ImGuiID       GetID(const void* ptr_id);

   IMGUI_API bool IsOver(OPERATION op);
   IMGUI_API void SetGizmoSizeClipSpace(float value);

   IMGUI_API void AllowAxisFlip(bool value);

   IMGUI_API void SetAxisLimit(float value);
   IMGUI_API void SetAxisMask(bool x, bool y, bool z);
   IMGUI_API void SetPlaneLimit(float value);
   IMGUI_API bool IsOver(float* position, float pixelRadius);

   enum COLOR
   {
      DIRECTION_X,      // directionColor[0]
      DIRECTION_Y,      // directionColor[1]
      DIRECTION_Z,      // directionColor[2]
      PLANE_X,          // planeColor[0]
      PLANE_Y,          // planeColor[1]
      PLANE_Z,          // planeColor[2]
      SELECTION,        // selectionColor
      INACTIVE,         // inactiveColor
      TRANSLATION_LINE, // translationLineColor
      SCALE_LINE,
      ROTATION_USING_BORDER,
      ROTATION_USING_FILL,
      HATCHED_AXIS_LINES,
      TEXT,
      TEXT_SHADOW,
      COUNT
   };

   struct Style
   {
      IMGUI_API Style();

      float TranslationLineThickness;   // Thickness of lines for translation gizmo
      float TranslationLineArrowSize;   // Size of arrow at the end of lines for translation gizmo
      float RotationLineThickness;      // Thickness of lines for rotation gizmo
      float RotationOuterLineThickness; // Thickness of line surrounding the rotation gizmo
      float ScaleLineThickness;         // Thickness of lines for scale gizmo
      float ScaleLineCircleSize;        // Size of circle at the end of lines for scale gizmo
      float HatchedAxisLineThickness;   // Thickness of hatched axis lines
      float CenterCircleSize;           // Size of circle at the center of the translate/scale gizmo

      ImVec4 Colors[COLOR::COUNT];
   };

   IMGUI_API Style& GetStyle();
}
