#pragma once
#include "gui_framework_includes.h"

class GUIGeom {
private:
	const ImVec2 windowSize;
public:
	GUIGeom(const ImVec2 _windowSize) : windowSize(_windowSize) { }

	ImVec2 CalculateCenter(ImVec2 itemSize, ImVec2 winSize = { 0, 0 });
	inline ImVec2 CalculateCenter(LPCSTR text, ImVec2 winSize = { 0, 0 }) {
		return CalculateCenter(ImGui::CalcTextSize(text), winSize);
	}

	float CalculateCenterX(float itemWidth, float winWidth = 0);
	inline float CalculateCenterX(LPCSTR text, float winWidth = 0) {
		return CalculateCenterX(ImGui::CalcTextSize(text).x, winWidth);
	}

	float CalculateCenterY(float itemHeight, float winHeight = 0);
	inline float CalculateCenterY(LPCSTR text, float winHeight = 0) {
		return CalculateCenterY(ImGui::CalcTextSize(text).y, winHeight);
	}


	void CalculateAndSetCenter(ImVec2 itemSize, ImVec2 winSize = { 0, 0 });
	inline void CalculateAndSetCenter(LPCSTR text, ImVec2 winSize = { 0, 0 }) {
		CalculateAndSetCenter(ImGui::CalcTextSize(text), winSize);
	}

	void CalculateAndSetCenterX(float itemWidth, float winWidth = 0);
	inline void CalculateAndSetCenterX(LPCSTR text, float winWidth = 0) {
		CalculateAndSetCenterX(ImGui::CalcTextSize(text).x, winWidth);
	}

	void CalculateAndSetCenterY(float itemHeight, float winHeight = 0);
	inline void CalculateAndSetCenterY(LPCSTR text, float winHeight = 0) {
		CalculateAndSetCenterY(ImGui::CalcTextSize(text).y, winHeight);
	}
};

extern const ImVec2 MENU_SIZE;
inline GUIGeom g_guiGeom(MENU_SIZE);