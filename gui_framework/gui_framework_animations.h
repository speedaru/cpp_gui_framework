#pragma once
#include "gui_framework_includes.h"
#include "gui_framework_colors.h"

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
