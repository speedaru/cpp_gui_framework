#pragma once
#include "gui_framework_includes.h"
#include "gui_framework_colors.h"

#pragma region types
typedef bool IsActive;
#pragma endregion // types

#pragma region animations
#define UPDATE_DELTA(var_opacity, var, speed) var_opacity = ImLerp(var_opacity, b_active ? var.active: b_hovered ? var.hovered : var.base, speed * (1.0f - dt))

struct FpUIAnimBase {
	struct Opacity {
		float active, hovered, base;
		Opacity() = default;
		Opacity(float a, float h, float b) : active(a), hovered(h), base(b) {}
	};

	// const lerp speeds
	static constexpr const float BG_SPEED{ 0.07f };
	static constexpr const float TEXT_SPEED{ BG_SPEED };

	// current values that depend on state
	float bg_opacity{};
	float text_opacity{};

	Opacity background{};
	Opacity text{};

	FpUIAnimBase() = default;

	FpUIAnimBase(
		Opacity bg,
		Opacity t
	) : background(bg),
		text(t) {
	}

	// update animation values. dt = DeltaTime
	inline virtual void update(bool b_active, bool b_hovered, float dt) {
		UPDATE_DELTA(bg_opacity, background, BG_SPEED);
		UPDATE_DELTA(text_opacity, text, BG_SPEED);
	}

	//// copy operator
	//void operator=(const FpUIAnimBase& rhs) {
	//	bg_opacity = rhs.bg_opacity; text_opacity = rhs.text_opacity;
	//	background = rhs.background; text = rhs.text;
	//}
};

struct FpUiAnimOutline {
	static constexpr const float OUTLINE_SPEED{ 0.15f };

	float outline_opacity{};

	FpUIAnimBase::Opacity outline{};

	FpUiAnimOutline() = default;

	FpUiAnimOutline(
		FpUIAnimBase::Opacity o
	) : outline(o) {
	}

	// update animation values. dt = DeltaTime
	inline virtual void update(bool b_active, bool b_hovered, float dt) {
		//printf("outline active, hovered, base: %d %d %d\n", b_active, b_hovered, !b_active && !b_hovered);
		UPDATE_DELTA(outline_opacity, outline, OUTLINE_SPEED);
	}

	//// copy operator
	//void operator=(const FpUiAnimOutline& rhs) {
	//	outline_opacity = rhs.outline_opacity;
	//	outline = rhs.outline;
	//}
};

struct FpUIAnimButton : public FpUIAnimBase, public FpUiAnimOutline {
	FpUIAnimButton() = default;

	FpUIAnimButton(
		Opacity bg,
		Opacity t,
		Opacity o
	) : FpUIAnimBase(bg, t), FpUiAnimOutline(o) {
	}

	// update animation values. dt = DeltaTime
	inline virtual void update(bool b_active, bool b_hovered, float dt) override {
		//printf("outline active, hovered, base: %d %d %d\n", b_active, b_hovered, !b_active && !b_hovered);
		UPDATE_DELTA(bg_opacity, background, BG_SPEED);
		UPDATE_DELTA(text_opacity, text, TEXT_SPEED);
		UPDATE_DELTA(outline_opacity, outline, OUTLINE_SPEED);
	}

	//// copy operator
	//void operator=(const FpUIAnimButton& rhs) {
	//	bg_opacity = rhs.bg_opacity; text_opacity = rhs.text_opacity; outline_opacity = rhs.outline_opacity;
	//	background = rhs.background; text = rhs.text; outline = rhs.outline;
	//}
};

// button with indicator
struct FpUIAnimButtonWithIndicator : public FpUIAnimButton {
	static constexpr const float INDICATOR_SPEED{ FpUIAnimBase::BG_SPEED };

	float indicator_opacity{};

	Opacity indicator{};

	FpUIAnimButtonWithIndicator() = default;

	FpUIAnimButtonWithIndicator(
		Opacity bg,
		Opacity t,
		Opacity o,
		Opacity i
	) : indicator(i),
		FpUIAnimButton(bg, t, o) {
	}

	// update animation values. dt = DeltaTime
	inline virtual void update(bool b_active, bool b_hovered, float dt) override {
		UPDATE_DELTA(bg_opacity, background, BG_SPEED);
		UPDATE_DELTA(text_opacity, text, TEXT_SPEED);
		UPDATE_DELTA(outline_opacity, outline, OUTLINE_SPEED);
	}

	inline void update_indicator(bool b_active, bool b_hovered, float dt) {
		UPDATE_DELTA(indicator_opacity, indicator, INDICATOR_SPEED);
	}

	//// copy operator
	//void operator=(const FpUIAnimButtonWithIndicator& rhs) {
	//	bg_opacity = rhs.bg_opacity; text_opacity = rhs.text_opacity; outline_opacity = rhs.outline_opacity; indicator_opacity = rhs.indicator_opacity;
	//	background = rhs.background; text = rhs.text; outline = rhs.outline; indicator = rhs.indicator;
	//}
};

template <typename T>
concept DerivedUiAnim = std::is_base_of_v<FpUIAnimBase, T>;
#pragma endregion // animations

#pragma region math_operators
static bool operator==(const FpUIAnimBase::Opacity& lhs, const FpUIAnimBase::Opacity& rhs) { return lhs.active == rhs.active && lhs.hovered == rhs.hovered && lhs.base == rhs.base; }

// FpUIAnimBase
static bool operator==(const FpUIAnimBase& lhs, const FpUIAnimBase& rhs) { return lhs.background == rhs.background && lhs.text == rhs.text; }
static bool operator!=(const FpUIAnimBase& lhs, const FpUIAnimBase& rhs) { return !(lhs == rhs); }
static bool operator!(const FpUIAnimBase& lhs) { return lhs == FpUIAnimBase({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }); }

// FpUIAnimButton
static bool operator==(const FpUIAnimButton& lhs, const FpUIAnimButton& rhs) { return (FpUIAnimBase)lhs == (FpUIAnimBase)rhs && lhs.outline == rhs.outline; }
static bool operator!=(const FpUIAnimButton& lhs, const FpUIAnimButton& rhs) { return !(lhs == rhs); }
static bool operator!(const FpUIAnimButton& lhs) { return lhs == FpUIAnimButton({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }); }

// FpUIAnimButtonIcon
static bool operator==(const FpUIAnimButtonWithIndicator& lhs, const FpUIAnimButtonWithIndicator& rhs) { return (FpUIAnimButton)lhs == (FpUIAnimButton)rhs && lhs.indicator == rhs.indicator; }
static bool operator!=(const FpUIAnimButtonWithIndicator& lhs, const FpUIAnimButtonWithIndicator& rhs) { return !(lhs == rhs); }
static bool operator!(const FpUIAnimButtonWithIndicator& lhs) { return lhs == FpUIAnimButtonWithIndicator({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }); }
#pragma endregion // math_operators

#pragma region elements_structs
// internal structs

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

// regular structs

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
#pragma endregion

namespace FP_GUI_DEFAULTS {
	namespace anims {
		namespace base {
			inline const FpUIAnimBase hidden({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
			inline const FpUIAnimBase none({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f });
		}
		namespace outlines {
			inline const FpUiAnimOutline none({ 1.f, 1.f, 1.f });
			inline const FpUiAnimOutline regular({ 1.f, 0.7f, 0.5f });
		}
		namespace buttons {
			inline const FpUIAnimButton hidden({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
			inline const FpUIAnimButton none({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f });
			inline const FpUIAnimButton regular({ 1.f, 0.6f, 0.4f }, { 1.f, 1.f, 0.7f }, { 1.f, 1.f, 0.8f });
		}
		namespace icon_ibuttons {
			inline const FpUIAnimButtonWithIndicator hidden({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f });
			inline const FpUIAnimButtonWithIndicator none({ 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f });
			static const FpUIAnimButtonWithIndicator sidebarButton({ 0.18f, 0.06f, 0.f }, { 1.f, 0.6f, 0.4f }, { 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f });
		}
	}

	namespace colors {
		inline const ImU32 buttonColor(IM_COL32(255, 104, 10, 102));
		inline const ImU32 defaultOutline(IM_COL32(255, 45, 21, 255));
	}
}