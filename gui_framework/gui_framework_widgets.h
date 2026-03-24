#pragma once
#include "gui_framework_includes.h"
#include "gui_framework_colors.h"

// internal structs with "_" prefix

struct _FpGuiOutline {
	ImU32 color{ IMGUI_COLORS::black };
	float thickness{};
};

struct _FpGuiButtonBase {
	const char* text{};
	float textSize{}; // 0 for default font size
	ImU32 textColor{ IMGUI_COLORS::white };
	ImU32 backgroundColor{ IMGUI_COLORS::white };
	_FpGuiOutline outline{};
	float rounding{};
};

// public structs

struct FpGuiRectOutlined {
	_FpGuiOutline outline{};
	float rounding{};
	const FpUiAnimOutline* panim;
};

struct FpGuiButton : _FpGuiButtonBase {
	const FpUIAnimButton* panim{};
};

struct FpGuiButtonIcon : FpGuiButton {
	ImFont* iconFont{};
};

struct FpGuiButtonIconWithIndicator : _FpGuiButtonBase {
	ImFont* iconFont{};
	ImU32 indicatorColor{ IMGUI_COLORS::white };
	ImVec2 indicatorPosOffset{}; // if +2, 0 will draw it to the right of the button
	ImVec2 indicatorSize{};
	float indicatorRounding{};
	bool invertRounding{};
	const FpUIAnimButtonWithIndicator* panim{};
};

struct FpGuiTextBox {
	const char* id; // must start with ##
	std::string buff{};
	_FpGuiOutline outline{};
	float rounding{};
};

struct FpGuiNotiLabel {
	float posY{};
	float windowWidth{};
};
