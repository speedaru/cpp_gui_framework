#pragma once
#include "gui_framework_includes.h"

#define NORMALIZE_COLOR1F(c) (float)(c / 255.f)
#define NORMALIZE_COLOR3F(r, g, b) IM_COL32(r, g, b, 255)

#define UNNORMALIZE_COLOR1F(c) (int)(c * 255.f)
#define UNNORMALIZE_COLOR4F(r, g, b, a) r * 255.f, g * 255.f, b * 255.f, a * 255.f

#define IMU32_GET_FIELD(color, fieldIndex) ((color & (0x000000FF << fieldIndex * 8)) >> fieldIndex * 8)
#define IMU32_SET_ALPHA(color, a) IM_COL32(IMU32_GET_FIELD(color, 0), IMU32_GET_FIELD(color, 1), IMU32_GET_FIELD(color, 2), a)
#define IMU32_FROM_VEC4(unnormalizedVec4) IM_COL32(unnormalizedVec4.x, unnormalizedVec4.y, unnormalizedVec4.z, unnormalizedVec4.w)
#define IMU32_TO_VEC4(c) ImVec4(NORMALIZE_COLOR1F(IMU32_GET_FIELD(c, 0)), NORMALIZE_COLOR1F(IMU32_GET_FIELD(c, 1)), NORMALIZE_COLOR1F(IMU32_GET_FIELD(c, 2)), NORMALIZE_COLOR1F(IMU32_GET_FIELD(c, 3)))

namespace IMGUI_COLORS {
#define ADD_COLOR4(name, r, g, b, a) static const ImU32 name = IM_COL32(r, g, b, a)
#define ADD_COLOR3(name, r, g, b) ADD_COLOR4(name, r, g, b, 255)

	// b & w
	ADD_COLOR3(black, 0, 0, 0);
	ADD_COLOR3(white, 255, 255, 255);

	// primary colors
	ADD_COLOR3(red, 255, 0 , 0);
	ADD_COLOR3(green, 0, 128, 0);
	ADD_COLOR3(blue, 0, 0, 255);
	
	// bright colors
	ADD_COLOR3(lime, 0, 255, 0);
}