#include <gui_framework.h>
#include <gui_framework_types.h>
#include <gui_framework_elements.h>
#include <gui_framework_fonts.h>
#include <gui_geom.h>
#include <gui_ext.h>

#include <base_window.h>
#include <render.h>
#include <gui_globals.h>

#include "QuicksandSemiBold.hpp"

// simple gui button (should put in a seperate file)
GuiElementWrapper<FpGuiButton> imguiButton = { ImVec2(100, 30), []() {
    FpGuiButton button;
    button.text = "button";
    button.textSize = 18.f;
    button.backgroundColor = FP_GUI_DEFAULTS::colors::buttonColor;
    button.rounding = 5.f;
    button.panim = &FP_GUI_DEFAULTS::anims::buttons::regular;
    return button;
}};

// fonts example
namespace fonts {
	// individual fonts
	inline FP_GUI::Font quicksand({ 18.f, 20.f, 24.f, 28.f, 32.f, 36.f });

	// Font, data, size descriptors
	inline const FP_GUI::FontDataPairs fonts = {
		{ quicksand, (void*)QuicksandSemiBold, sizeof(QuicksandSemiBold) },
	};
}

// super simple gui window class
class Gui : public BaseWindow {
public:
    Gui(DxDevice& dxDevice, bool* pIsOpen) : BaseWindow(pIsOpen) {
		m_windowData.menuName = g_windowTitle;
		m_windowData.className = g_windowTitle;
		m_windowData.styles = WS_POPUP;
		m_windowData.startPos = { g_screenSize.x / 2 - g_windowSize.x / 2, g_screenSize.y / 2 - g_windowSize.y / 2 };
		m_windowData.size = g_windowSize;

		Init(dxDevice, GuiWndProcHandler);
	}

    void CreateImGui() override {
        BaseWindow::CreateImGui();

        // disable imgui.ini file
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;
    }

    void StartRender() override {
        BaseWindow::StartRender();
        ImGui::PushFont(fonts::quicksand.GetFont(18.f));
    }

    void Render() override {
		ImGui::SetNextWindowPos({ 0.f, 0.f });
		ImGui::SetNextWindowSize({ (float)m_windowData.size.x, (float)m_windowData.size.y });
		ImGui::Begin("my menu", m_pIsOpen,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove);

		ImGui::Text("press DEL to toggle overlay");

        static GuiElement<FpGuiButton>& button = imguiButton.Get();
        g_guiGeom.CalculateAndSetCenterX(button.size.x);
        if (button.active = FP_GUI::Button(button.active, button.size, button.params)) {
            printf("button pressed\n");
        }

		ImGui::End();
    }

    void EndRender() override {
        ImGui::PopFont();
        BaseWindow::EndRender();
    }
};


// simple gui app exaple
// features:
// - borderless window
// - supear easy custom fonts
// - easy to use
// - easy to configure widgets
int main() {
    bool isWindowOpen = true;
    DxDevice dxDevice;

    // get screen size into global variables
    GuiDefinitionsInit();

    // create d3d device and gui window
    render::CreateD3DDevice(dxDevice);
    Gui gui(dxDevice, &isWindowOpen);

    // init fonts after imgui created in gui window
    FP_GUI::InitFonts(ImGui::GetIO(), fonts::fonts);

    // render gui window
    while (isWindowOpen) {
        gui.StartRender();
        gui.Render();
        gui.EndRender();
    }
}

