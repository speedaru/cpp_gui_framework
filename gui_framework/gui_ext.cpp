#include "gui_ext.h"

void gui_ext::TextEx(ImFont* font, const char* format, ...) noexcept {
	va_list args;
	va_start(args, format);

	char text[256]{};
	vsnprintf_s(text, sizeof(text), sizeof(text) - 1, format, args);

	va_end(args);

	ImGui::PushFont(font);
	ImGui::Text(text);
	ImGui::PopFont();
}

bool gui_ext::ButtonEx(ImFont* font, const char* label, const ImVec2& buttonSize) noexcept {
	ImGui::PushFont(font);
	bool res = ImGui::Button(label, buttonSize);
	ImGui::PopFont();

	return res;
}

ID3D11ShaderResourceView* gui_ext::LoadResource(const uint8_t* imageBytes, size_t imageSize) {
    ID3D11Device* device = gui_vars::g_pd3dDevice;
    ID3D11DeviceContext* context = gui_vars::g_pd3dDeviceContext;

    IWICImagingFactory* pWIC = nullptr;
    IWICStream* pStream = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;

    ID3D11Texture2D* pTexture = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr;

    // Create WIC factory
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory, (LPVOID*)&pWIC);
    if (FAILED(hr)) return nullptr;

    // Create WIC stream from memory
    hr = pWIC->CreateStream(&pStream);
    if (FAILED(hr)) { pWIC->Release(); return nullptr; }

    hr = pStream->InitializeFromMemory((WICInProcPointer)imageBytes, (DWORD)imageSize);
    if (FAILED(hr)) { pStream->Release(); pWIC->Release(); return nullptr; }

    // Decode stream
    hr = pWIC->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder);
    if (FAILED(hr)) { pStream->Release(); pWIC->Release(); return nullptr; }

    // Get frame
    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr)) { pDecoder->Release(); pStream->Release(); pWIC->Release(); return nullptr; }

    // Convert to RGBA 32bpp
    hr = pWIC->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) { pFrame->Release(); pDecoder->Release(); pStream->Release(); pWIC->Release(); return nullptr; }

    hr = pConverter->Initialize(
        pFrame, GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.f,
        WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) { pConverter->Release(); pFrame->Release(); pDecoder->Release(); pStream->Release(); pWIC->Release(); return nullptr; }

    // Get image size
    UINT width, height;
    pConverter->GetSize(&width, &height);

    // Copy pixels
    std::vector<BYTE> pixels(width * height * 4);
    hr = pConverter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());
    if (FAILED(hr)) { pConverter->Release(); pFrame->Release(); pDecoder->Release(); pStream->Release(); pWIC->Release(); return nullptr; }

    // Describe texture
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = pixels.data();
    initData.SysMemPitch = width * 4;

    // Create texture
    hr = device->CreateTexture2D(&desc, &initData, &pTexture);
    if (FAILED(hr)) { pConverter->Release(); pFrame->Release(); pDecoder->Release(); pStream->Release(); pWIC->Release(); return nullptr; }

    // Create SRV
    hr = device->CreateShaderResourceView(pTexture, nullptr, &pSRV);

    // Cleanup
    pTexture->Release();
    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pStream->Release();
    pWIC->Release();

    if (FAILED(hr)) return nullptr;
    return pSRV;
}
void gui_ext::Image(const ID3D11ShaderResourceView* image, const ImVec2& size, const ImVec2& pos) {
    ImGui::SetCursorPos(pos);
    ImGui::Image((ImTextureID)image, size);
}

void gui_ext::IsWidgetHoveredOrPressed(const ImRect& rect, ImGuiID id, bool* hovered, bool* pressed) {
    if (!hovered) {
        return;
    }

    *hovered = ImGui::ItemHoverable(rect, id, 0);
    if (pressed) {
        *pressed = *hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left, ImGuiKeyOwner_Any, 0);
    }
}

ImGuiID gui_ext::GetImGuiID(const char* itemId) {
    return ImGui::GetCurrentWindow()->GetID(itemId);
}
