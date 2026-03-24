#pragma once
#include <vector>
#include <unordered_map>
#include <imgui/imgui.h>

namespace FP_GUI {
	class Font {
	public:
		Font(const std::vector<float>& sizes);
		void Init(ImGuiIO& io, void* fontData, int fontDataSize);
		ImFont* GetFont(float size);

		inline bool Initialized() const { return m_initialized; }

	private:
		std::unordered_map<float, ImFont*> m_fonts;
		bool m_initialized = false;
	};

	struct FontDataPair {
		Font& font;
		void* fontData;
		int fontDataSize;
	};
	using FontDataPairs = std::vector<FP_GUI::FontDataPair>;

	void InitFonts(ImGuiIO& io, const FontDataPairs& fonts);
}
