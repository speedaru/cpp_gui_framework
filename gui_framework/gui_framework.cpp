#include "gui_framework.h"

#include "gui_ext.h"
#include "gui_geom.h"


static inline float GetExtraRounding(float thickness) {
	return std::roundf(thickness / 2.f);
}


// helper to retrieve or create an animation entry
template <DerivedUiAnim T>
void FP_GUI::SetAnim(ImGuiID id, const T& animation, bool forceCreate) {
	if (!forceCreate && g_animations.find(id) != g_animations.end()) { // not forcecreate and an animation alr exists
		return;
	}

	//printf("[SetAnim] new shared ptr\n");
	g_animations[id] = std::make_shared<T>(animation);
}
template <DerivedUiAnim T>
std::shared_ptr<FpUIAnimBase> FP_GUI::GetAnim(ImGuiID id, const T& defaultAnimation) {
	// create new if not exists
	if (g_animations.find(id) == g_animations.end()) {
		//printf("[GetAnim] new shared ptr\n");
		g_animations[id] = std::make_shared<T>(defaultAnimation);
	}

	return g_animations.at(id);
}

void FP_GUI::RectOutlinedEx(ImRect rect, const FpGuiRectOutlined& params, bool drawOutlineOutside) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	// otherwise black gaps when too thick and round
	float extraRounding = 0;

	if (drawOutlineOutside) {
		// use floor so if thickness is even it will be closer to cursor than further
		const float thickness = std::floorf(params.outline.thickness / 2.f);
		rect.Min.x -= thickness;
		rect.Min.y -= thickness;
		rect.Max.x += thickness;
		rect.Max.y += thickness;

		extraRounding = thickness;
	}

	window->DrawList->AddRect(rect.Min, rect.Max,
		IMU32_SET_ALPHA(params.outline.color, UNNORMALIZE_COLOR1F(params.panim->outline_opacity)),
		params.rounding + extraRounding, ImDrawFlags_RoundCornersAll, params.outline.thickness
	);
}
void FP_GUI::RectOutlined(const ImVec2& size, const FpGuiRectOutlined& params, bool drawOutlineOutside) {
	// start drawing outline
	ImVec2 pos = ImGui::GetWindowPos() + ImGui::GetCursorPos();
	const ImRect rect{ pos, pos + size };

	return RectOutlinedEx(rect, params, drawOutlineOutside);
}

static void _DrawButtonBorder(ImRect rect, const FpGuiRectOutlined& params) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiIO& io = ImGui::GetIO();

	// make rect bigger than the actual button rect bcs we're drawing just an outline
	const float thickness = params.outline.thickness;
	rect.Min.x -= thickness;
	rect.Min.y -= thickness;
	rect.Max.x += thickness;
	rect.Max.y += thickness;

	// bcs rect becomes less round when growig
	const float extraRounding = GetExtraRounding(thickness);

	const FpUiAnimOutline* panim = params.panim;
	if (!panim) {
		panim = &FP_GUI_DEFAULTS::anims::outlines::none;
	}

	window->DrawList->AddRectFilled(rect.Min, rect.Max,
		IMU32_SET_ALPHA(params.outline.color, UNNORMALIZE_COLOR1F(panim->outline_opacity)),
		params.rounding + extraRounding
	);
}
IsActive FP_GUI::ButtonEx(bool active, const ImRect& rect, const FpGuiButton& params) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiIO& io = ImGui::GetIO();

	// interaction
	bool hovered, pressed;
	ImGuiID id = window->GetID(params.text);
	//bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, 0);
	gui_ext::IsWidgetHoveredOrPressed(rect, id, &hovered, &pressed);

	if (params.panim) { // set new anim
		SetAnim(id, *params.panim);
	}

	std::shared_ptr<FpUIAnimButton> panim = std::dynamic_pointer_cast<FpUIAnimButton>(GetAnim(id, FP_GUI_DEFAULTS::anims::buttons::regular));
	if (!panim) {
		printf("failed to get animation for button\n");
		return false;
	}

	// animate button depending on state
	panim->update(pressed || active, hovered, io.DeltaTime);

	// draw background
	window->DrawList->AddRectFilled(rect.Min, rect.Max,
		IMU32_SET_ALPHA(params.backgroundColor, UNNORMALIZE_COLOR1F(panim->bg_opacity)),
		params.rounding
	);

	// draw text
	float fontSize = ImGui::GetFontSize();
	ImVec2 textSize = ImGui::CalcTextSize(params.text);

	if (params.textSize != 0) { // specified textsize
		fontSize = params.textSize;
		textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, params.text);
		textSize.y *= 1.2f;
	}

	ImVec2 textPos = (rect.Min + rect.Max - textSize) * 0.5f;

	window->DrawList->AddText(
		ImGui::GetFont(),
		fontSize,
		textPos,
		IMU32_SET_ALPHA(params.textColor, UNNORMALIZE_COLOR1F(panim->text_opacity)),
		params.text
	);

	// draw outline
	if (params.outline.thickness > 0) {
		FpGuiRectOutlined outlineParams(params.outline, params.rounding, panim.get());
		//_DrawButtonBorder(rect, outlineParams);
		RectOutlinedEx(rect, outlineParams, true);
	}

	return pressed;
}
IsActive FP_GUI::Button(bool active, const ImVec2& size, const FpGuiButton& params) {
	const ImVec2 pos = ImGui::GetWindowPos() + ImGui::GetCursorPos();
	const ImRect rect = { pos, pos + size };

	return ButtonEx(active, rect, params);
}

IsActive FP_GUI::ButtonIconEx(bool active, const ImRect& rect, const FpGuiButtonIcon& params) {
	ImGui::PushFont(params.iconFont);
	IsActive res = ButtonEx(active, rect, params);
	ImGui::PopFont();

	return res;
}
IsActive FP_GUI::ButtonIcon(bool active, const ImVec2& size, const FpGuiButtonIcon& params) {
	const ImVec2 pos = ImGui::GetWindowPos() + ImGui::GetCursorPos();
	const ImRect rect = { pos, pos + size };

	return ButtonIconEx(active, rect, params);
}

IsActive FP_GUI::ButtonIconWithIndicatorEx(bool active, const ImRect& rect, const FpGuiButtonIconWithIndicator& params) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiIO& io = ImGui::GetIO();

	// interaction
	bool hovered, held;
	ImGuiID id = window->GetID(params.text);
	bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, 0);

	if (params.panim) { // set new anim
		SetAnim(id, *params.panim);
	}

	std::shared_ptr<FpUIAnimButtonWithIndicator> panim = std::dynamic_pointer_cast<FpUIAnimButtonWithIndicator>(GetAnim(id, FP_GUI_DEFAULTS::anims::icon_ibuttons::none));
	if (!panim) {
		printf("failed to get animation for button icon with indicator\n");
		return false;
	}

	// animate only indicator bcs ButtonIconEx animates rest of button
	panim->update_indicator(active, hovered, io.DeltaTime);

	// draw icon button
	FpGuiButtonIcon buttonIconParams; // down cast to get only icon anim
	buttonIconParams.text = params.text;
	buttonIconParams.textSize = params.textSize;
	buttonIconParams.textColor = params.textColor;
	buttonIconParams.backgroundColor = params.backgroundColor;
	buttonIconParams.outline = params.outline;
	buttonIconParams.rounding = params.rounding;
	buttonIconParams.iconFont = params.iconFont;
	buttonIconParams.panim = params.panim;
	bool newIsActive = ButtonIconEx(active, rect, buttonIconParams);

	// parse indicator params
	ImDrawFlags indicatorDrawFlags = ImDrawFlags_RoundCornersNone;
	ImVec2 buttonSize = rect.GetSize();

	ImVec2 sideSigns(params.indicatorPosOffset.x > 0 ? 1.f : params.indicatorPosOffset.x < 0 ? -1.f : 0.f,
		params.indicatorPosOffset.y > 0 ? 1.f : params.indicatorPosOffset.y < 0 ? -1.f : 0.f);
	ImVec2 padding = params.indicatorPosOffset;
	ImVec2 size = params.indicatorSize;
	ImVec2 indicatorPos = rect.Min + rect.GetSize() / 2;  // middle of button

	// check X
	if (params.indicatorPosOffset.x > 0) {
		indicatorDrawFlags |= params.invertRounding ? ImDrawFlags_RoundCornersLeft : ImDrawFlags_RoundCornersRight;
	}
	else if (params.indicatorPosOffset.x < 0) {
		indicatorDrawFlags |= params.invertRounding ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersLeft;
		indicatorPos.x -= size.x;
	}

	// check Y
	if (params.indicatorPosOffset.y > 0) {
		indicatorDrawFlags |= params.invertRounding ? ImDrawFlags_RoundCornersTop : ImDrawFlags_RoundCornersBottom;
	}
	else if (params.indicatorPosOffset.y < 0) {
		indicatorDrawFlags |= params.invertRounding ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersTop;
		indicatorPos.y -= size.y;
	}

	indicatorPos += sideSigns * (rect.GetSize() / 2 + padding); // add/sub padding + buttonsize

	if (params.indicatorPosOffset.x) { // if drawing to the left or right then make it so indicator is in middle of button
		indicatorPos.y -= size.y / 2;
	}
	if (params.indicatorPosOffset.y) {
		indicatorPos.x -= size.x / 2;
	}

	window->DrawList->AddRectFilled(indicatorPos, indicatorPos + size,
		IMU32_SET_ALPHA(params.indicatorColor, UNNORMALIZE_COLOR1F(panim->indicator_opacity)), params.indicatorRounding, indicatorDrawFlags);

	return newIsActive;
}
IsActive FP_GUI::ButtonIconWithIndicator(bool active, const ImVec2& buttonSize, const FpGuiButtonIconWithIndicator& params) {
	const ImVec2 pos = ImGui::GetWindowPos() + ImGui::GetCursorPos();
	const ImRect rect = { pos, pos + buttonSize };

	//printf("(hovered indicator: %f) bg opacity: %d\n", params.anim.indicator.hovered, UNNORMALIZE_COLOR1F(params.anim.bg_opacity));
	return ButtonIconWithIndicatorEx(active, rect, params);
}

static int TextBoxBuffResizeCallback(ImGuiInputTextCallbackData* data) {
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
		std::string* buff = reinterpret_cast<std::string*>(data->UserData);
		buff->resize((size_t)(data->BufTextLen));
		data->Buf = buff->data();
	}
	return 0;
}
void FP_GUI::TextBox(float width, FpGuiTextBox& params) {
	ImGui::SetNextItemWidth(width);

	IMGUI_STYLEVAR_BLOCK(ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, params.rounding, {
		ImGui::InputText(params.id, params.buff.data(), params.buff.capacity(), ImGuiInputTextFlags_CallbackResize, TextBoxBuffResizeCallback, &params.buff);
	})

	ImRect rect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

	// draw outline
	if (params.outline.thickness > 0) {
		static FpUiAnimOutline outlineAnim = FP_GUI_DEFAULTS::anims::outlines::regular;
		FpGuiRectOutlined outlineParams(params.outline, params.rounding, &outlineAnim);

		// interaction
		bool hovered, pressed;
		gui_ext::IsWidgetHoveredOrPressed(rect, gui_ext::GetImGuiID(params.id), &hovered, &pressed);

		outlineAnim.update(pressed, hovered, ImGui::GetIO().DeltaTime);

		RectOutlinedEx(rect, outlineParams, true);
	}
}

void FP_GUI::NotiLabel(const std::string& text, ImU32 color, const FpGuiNotiLabel& params) {
	ImGui::SetCursorPosY(params.posY);

	guiGeom.CalculateAndSetCenterX(text.c_str(), params.windowWidth);

	ImGui::TextColored(IMU32_TO_VEC4(color), text.c_str());
}
