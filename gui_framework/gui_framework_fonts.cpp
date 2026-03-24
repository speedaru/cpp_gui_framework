#include "gui_framework_fonts.h"

FP_GUI::Font::Font(const std::vector<float>& sizes) {
	// init map with sizes and null ptrs
	m_fonts.reserve(sizes.size());
	for (float size : sizes) {
		m_fonts[size] = nullptr;
	}
}

void FP_GUI::Font::Init(ImGuiIO& io, void* fontData, int fontDataSize) {
	for (auto& [size, _] : m_fonts) {
		// if font already initialized skip it
		if (m_fonts[size]) {
			continue;
		}

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false; // required by imgui
		m_fonts[size] = io.Fonts->AddFontFromMemoryTTF(fontData, fontDataSize, size, &font_cfg);
	}

	m_initialized = true;
}

ImFont* FP_GUI::Font::GetFont(float size) {
	ImFont* res = m_fonts[size];
	assert(res != nullptr);

	return res;
}


void FP_GUI::InitFonts(ImGuiIO& io, const FontDataPairs& fonts) {
	for (const auto& font : fonts) {
		if (!font.font.Initialized()) {
			font.font.Init(io, font.fontData, font.fontDataSize);
		}
	}
}
