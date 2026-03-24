#pragma once
#include <iostream>
#include <dwmapi.h>
#include <d3d11.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

#include "gui_definitions.h"
#include "render.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

LRESULT ImGuiWndProcHandler(HWND hwnd, UINT message, WPARAM wideParameter, LPARAM longParameter);
LRESULT GuiWndProcHandler(HWND hwnd, UINT message, WPARAM wideParameter, LPARAM longParameter);


class BaseWindow {
public:
    BaseWindow(bool* pIsOpen);
    ~BaseWindow();

    // creates window, direct 3d device, imgui ctx, initalizes fonts
    void Init(DxDevice& dxDevice, WNDPROC lpfnWndProc);

    virtual void CreateWindowClass();
    virtual void CreateImGui();

    virtual void StartRender();
    virtual void EndRender();
    virtual void Render() = 0;

    inline const DxDevice* GetDxDevice() const {
        return m_dxData.device;
    }

    inline POINTS* GetPWinLoc() {
        return &m_windowData.winLoc;
    }

protected:
    bool* m_pIsOpen{};
    DxData m_dxData{};
    WindowData m_windowData{};
    ImGuiData m_imGuiData{};
};
