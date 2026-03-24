#pragma once
#include <Windows.h>
#include "gui_definitions.h"
#include "gui_framework_fonts.h"

inline vec2 g_screenSize{};
inline vec2 g_windowSize{ 400, 300 };
inline const wchar_t* g_windowTitle = L"my sample app";


inline void GuiDefinitionsInit() {
	// already initialized
	if (g_screenSize.x != 0 && g_screenSize.y != 0) {
		return;
	}

	// get screen size
	RECT desktop{};
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	g_screenSize = { (int)desktop.right, (int)desktop.bottom };
}
