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

