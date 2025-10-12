#pragma once
#include <iostream>
#include <vector>

#include <d3d11.h>
#include <wincodec.h>   // For WIC
#pragma comment(lib, "windowscodecs.lib")

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


#define IMGUI_FONT_BLOCK(p_font, code) \
	ImGui::PushFont(p_font); \
	code \
	ImGui::PopFont();

#define IMGUI_STYLEVAR_BLOCK(style, value, code) \
	ImGui::PushStyleVar(style, value); \
	code \
	ImGui::PopStyleVar();


namespace gui_vars {
	extern ID3D11Device* g_pd3dDevice;
	extern ID3D11DeviceContext* g_pd3dDeviceContext;
};

namespace gui_ext {
	inline void PadY(float padding) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
	}

	void TextEx(ImFont* font, const char* format, ...) noexcept;
	bool ButtonEx(ImFont* font, const char* label, const ImVec2& buttonSize) noexcept;

	ID3D11ShaderResourceView* LoadResource(const uint8_t* imageBytes, size_t imageSize);
	void Image(const ID3D11ShaderResourceView* image, const ImVec2& size, const ImVec2& pos);

	void IsWidgetHoveredOrPressed(const ImRect& rect, ImGuiID id, bool* hovered, bool* pressed);
	ImGuiID GetImGuiID(const char* itemId);
}
