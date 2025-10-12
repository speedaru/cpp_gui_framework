#pragma once
#include <utility>
#include <functional>

#include "gui_framework.h"


template <typename T>
struct GuiElement {
	bool active{};
	ImVec2 size{};
	T params{};

	GuiElement() = default;
	GuiElement(const ImVec2& _size, const T& _params) : size(_size), params(_params) {
	}
};

template <typename T>
class GuiElementWrapper {
private:
	const ImVec2 size{};
	const std::function<T()> builder;
	mutable GuiElement<T> instance{};
	mutable bool initialized = false;

public:
	GuiElementWrapper(const ImVec2& _size, const std::function<T()>& _builder) : size(_size), builder(std::move(_builder)) {
	}

	inline GuiElement<T>& Get() const {
		if (!initialized) {
			instance = GuiElement<T>(size, builder());
			initialized = true;
		}
		return instance;
	}
};



// SAMPLE ELEMENTS
namespace gui_elements {
	namespace buttons {
		inline const GuiElementWrapper<FpGuiButton> loginButton(ImVec2(150, 35), []() {
			FpGuiButton button;
			button.text = "ok";
			button.textColor = IMGUI_COLORS::white;
			button.backgroundColor = FP_GUI_DEFAULTS::colors::buttonColor;
			button.outline = _FpGuiOutline(FP_GUI_DEFAULTS::colors::defaultOutline, 2.f); // red orange
			button.rounding = 6.f;
			button.panim = &FP_GUI_DEFAULTS::anims::buttons::regular;
			return button;
		});

		inline const GuiElementWrapper<FpGuiButton> button1(ImVec2(200, 40), []() {
			FpGuiButton button;
			button.text = "my_button";
			button.textColor = IMGUI_COLORS::white;
			button.backgroundColor = FP_GUI_DEFAULTS::colors::buttonColor;
			button.outline = _FpGuiOutline(FP_GUI_DEFAULTS::colors::defaultOutline, 2.f); // red orange
			button.rounding = 6.f;
			button.panim = &FP_GUI_DEFAULTS::anims::buttons::regular;
			return button;
		});
	}

	namespace textboxes {
		inline const GuiElementWrapper<FpGuiTextBox> loginLicense(ImVec2(300, 0), []() {
			FpGuiTextBox tb;
			tb.buff = "";
			tb.id = "##login_textbox";
			tb.outline = { FP_GUI_DEFAULTS::colors::defaultOutline, 2.f };
			tb.rounding = 6.f;
			return tb;
		});
	}

	namespace notilabels {
		inline const GuiElementWrapper<FpGuiNotiLabel> login(ImVec2(0, 0), []() {
			FpGuiNotiLabel label;
			label.posY = ImGui::GetCursorPosY() + 5;
			label.windowWidth = 400.f;
			return label;
		});
	}
}