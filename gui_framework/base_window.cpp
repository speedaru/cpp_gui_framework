#include "base_window.h"
#include <iostream>

#include "gui_globals.h"


LRESULT ImGuiWndProcHandler(HWND hwnd, UINT message, WPARAM wideParameter, LPARAM longParameter) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wideParameter, longParameter)) {
		return true;
	}

	return DefWindowProcW(hwnd, message, wideParameter, longParameter);
}

LRESULT GuiWndProcHandler(HWND hwnd, UINT message, WPARAM wideParameter, LPARAM longParameter) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wideParameter, longParameter)) {
		return true;
	}

	BaseWindow* pWindow = reinterpret_cast<BaseWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	POINTS* pWinLoc = pWindow->GetPWinLoc();

	switch (message) {
	case WM_SYSCOMMAND:
		if ((wideParameter & 0xfff0) == SC_KEYMENU) { // disable alt application menu
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		*pWinLoc = MAKEPOINTS(longParameter); // set click points
		break;
	case WM_MOUSEMOVE:
		if (wideParameter == MK_LBUTTON) {
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(hwnd, &rect);

			rect.left += points.x - pWinLoc->x;
			rect.top += points.y - pWinLoc->y;

			if (pWinLoc->x >= 0 && pWinLoc->x <= g_windowSize.x && pWinLoc->y >= 0 && pWinLoc->y <= g_windowSize.y) {
				SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			}
		}
		return 0;
	}

	return DefWindowProcW(hwnd, message, wideParameter, longParameter);
}


BaseWindow::BaseWindow(bool* pIsOpen) : m_pIsOpen(pIsOpen) {
	assert(m_pIsOpen != nullptr);
}

BaseWindow::~BaseWindow() {
	if (!m_pIsOpen) {
		printf("destructed uninitialised instance\n");
		return;
	}

	render::DestroyImGui(m_imGuiData);
	render::CleanupD3DData(m_dxData);
	render::DestroyWindowClass(m_windowData);
}

void BaseWindow::Init(DxDevice& dxDevice, WNDPROC lpfnWndProc) {
	render::CreateWindowClass(m_windowData, lpfnWndProc);
	CreateWindowClass();

	render::CreateD3DData(m_dxData, dxDevice, m_windowData);

	CreateImGui();
	render::CreateImGui(m_dxData, m_windowData, m_imGuiData);
}


void BaseWindow::CreateWindowClass() {
	if (m_windowData.windowClass.lpfnWndProc == GuiWndProcHandler) {
		SetWindowLongPtrW(m_windowData.hwnd, GWLP_USERDATA, (LONG_PTR)this);
	}

    // Show the window
    ::ShowWindow(m_windowData.hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_windowData.hwnd);
}

void BaseWindow::CreateImGui() {
	IMGUI_CHECKVERSION();
	ImGuiContext* context = m_imGuiData.context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
}

void BaseWindow::StartRender() {
	ImGui::SetCurrentContext(m_imGuiData.context);

	// Poll and handle messages (inputs, window resize, etc.)
	// See the WndProc() function below for our to dispatch events to the Win32 backend.
	MSG msg;
	while (::PeekMessageW(&msg, m_windowData.hwnd, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
		if (msg.message == WM_QUIT || msg.message == WM_DESTROY) {
			*m_pIsOpen = false;
		}
	}

	// check if window closed
	if (!IsWindow(m_windowData.hwnd) || !*m_pIsOpen) {
	    *m_pIsOpen = false;
		return;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void BaseWindow::EndRender() {
	ImGui::Render();
	m_dxData.device->pd3dDeviceContext->OMSetRenderTargets(1, &m_dxData.pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_dxData.pSwapChain->Present(1, 0);
}
