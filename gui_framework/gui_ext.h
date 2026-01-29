#pragma once
#include <iostream>
#include <vector>

#include <d3d11.h>

#include "gui_framework_includes.h"


#define IMGUI_FONT_BLOCK(p_font, code) \
	ImGui::PushFont(p_font); \
	code \
	ImGui::PopFont();

#define IMGUI_STYLEVAR_BLOCK(style, value, code) \
	ImGui::PushStyleVar(style, value); \
	code \
	ImGui::PopStyleVar();


namespace gui_ext {
	inline void PadY(float padding) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
	}

	ID3D11ShaderResourceView* LoadResource(const uint8_t* imageBytes, size_t imageSize, ID3D11Device* device, ID3D11DeviceContext* context);
	void Image(const ID3D11ShaderResourceView* image, const ImVec2& size, const ImVec2& pos);

	void IsWidgetHoveredOrPressed(const ImRect& rect, ImGuiID id, bool* hovered, bool* pressed);
	ImGuiID GetImGuiID(const char* itemId);
}
