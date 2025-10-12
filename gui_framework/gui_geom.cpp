#include "gui_geom.h"


ImVec2 GUIGeom::CalculateCenter(ImVec2 itemSize, ImVec2 winSize) {
	// if no specifc subwindow size specified then use whole imgui window size by default
	if (!winSize) {		// { 0, 0}
		winSize = windowSize;
	}

	return ImVec2(CalculateCenterX(itemSize.x, winSize.x), CalculateCenterY(itemSize.y, winSize.y));
}
float GUIGeom::CalculateCenterX(float itemWidth, float winWidth) {
	if (!winWidth) {
		winWidth = windowSize.x;
	}

	return (winWidth / 2) - (itemWidth / 2);
}
float GUIGeom::CalculateCenterY(float itemHeight, float winHeight) {
	if (!winHeight) {
		winHeight = windowSize.y;
	}

	return (winHeight / 2) - (itemHeight / 1.5f);
}

void GUIGeom::CalculateAndSetCenter(ImVec2 itemSize, ImVec2 winSize) {
	ImGui::SetCursorPos(CalculateCenter(itemSize, winSize));
}

void GUIGeom::CalculateAndSetCenterX(float itemWidth, float winWidth) {
	ImGui::SetCursorPosX(CalculateCenterX(itemWidth, winWidth));
}

void GUIGeom::CalculateAndSetCenterY(float itemHeight, float winHeight) {
	ImGui::SetCursorPosY(CalculateCenterY(itemHeight, winHeight));
}
