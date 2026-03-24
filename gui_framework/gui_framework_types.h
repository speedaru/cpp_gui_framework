#pragma once
#include "gui_framework_animations.h"
#include "gui_framework_widgets.h"

#pragma region types
typedef bool IsActive;
#pragma endregion // types

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