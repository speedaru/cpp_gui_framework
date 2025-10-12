#pragma once
#include <unordered_map>

#include "gui_framework_types.hpp"
#include "gui_framework_colors.h"


namespace FP_GUI {
    inline std::unordered_map<ImGuiID, std::shared_ptr<FpUIAnimBase>> g_animations;

    template <DerivedUiAnim T>
    void SetAnim(ImGuiID id, const T& animation, bool forceCreate = false);
    template <DerivedUiAnim T>
    std::shared_ptr<FpUIAnimBase> GetAnim(ImGuiID id, const T& defaultAnimation);

    // drawOutlineOutside: if true will make it so the outline is only outside the imgui cursor pos
    void RectOutlinedEx(ImRect rect, const FpGuiRectOutlined& params, bool drawOutlineOutside = false);
    // drawOutlineOutside: if true will make it so the outline is only outside the imgui cursor pos
    void RectOutlined(const ImVec2& size, const FpGuiRectOutlined& params, bool drawOutlineOutside = false);

    IsActive ButtonEx(bool active, const ImRect& rect, const FpGuiButton& params);
    IsActive Button(bool active, const ImVec2& size, const FpGuiButton& params);

    IsActive ButtonIconEx(bool active, const ImRect& rect, const FpGuiButtonIcon& params);
    IsActive ButtonIcon(bool active, const ImVec2& size, const FpGuiButtonIcon& params);

    IsActive ButtonIconWithIndicatorEx(bool active, const ImRect& rect, const FpGuiButtonIconWithIndicator& params);
    IsActive ButtonIconWithIndicator(bool active, const ImVec2& buttonSize, const FpGuiButtonIconWithIndicator& params);

    void TextBox(float width, FpGuiTextBox& params);

    void NotiLabel(const std::string& text, ImU32 color, const FpGuiNotiLabel& params);
}