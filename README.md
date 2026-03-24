# **🧩 FP GUI Framework**

A high-performance, object-oriented **C++ GUI framework** built on top of [Dear ImGui](https://github.com/ocornut/imgui). Designed for developers who want to bypass Win32/DirectX 11 boilerplate and focus on building modular, animated, and visually polished interfaces.

## **🌟 Key Features**

* **Window Abstraction**: Inherit from BaseWindow to automate Win32 class registration, D3D11 device setup, and ImGui context handling.  
* **Component Architecture**: Define UI elements (Buttons, TextBoxes, Icons) as persistent objects with their own state, styling, and animations.  
* **Smooth Animations**: Integrated animation system for hover effects, transitions, and active states.  
* **Font Management**: A Font class that handles multi-size loading from memory headers or files.  
* **Geometry Helpers**: Utilities for centering and layout calculations drawing.

## **📁 Core Architecture**

| Component | Description |
| :---- | :---- |
| **BaseWindow** | The foundation class for creating Win32 windows with DX11 backends. |
| **GuiElementWrapper** | A template-based system for declaring persistent UI components. |
| **FP\_GUI::Font** | Advanced font loader and retriever for dynamic scaling. |
| **gui\_globals.h** | Central configuration for application metadata. |
| **gui\_geom.h** | Layout utilities (e.g., CalculateAndSetCenterX). |

## **🚀 Getting Started**

### **1\. Global Configuration**

The framework relies on gui\_globals.h to define the initial state of your application. Set these before initializing your windows.

** gui\_globals.h **
```
inline vec2 g_screenSize{}; // filled automatically by calling GuiDefinitionsInit() in your main()
inline vec2 g_windowSize{ 400, 300 }; // to change
inline const wchar_t* g_windowTitle = L"my sample app"; // to change (used for creating Win32 window)
````

### **2\. Implementing a Custom Window**

Inherit from BaseWindow to build your UI. The framework handles the loop; you just provide the Render() logic.

```
\#include \<base\_window.h\>  
\#include \<gui\_framework.h\>

class MainMenu : public BaseWindow {  
public:
    MainMenu(DxDevice& dxDevice, bool\* pIsOpen) : BaseWindow(pIsOpen) {  
        m\_windowData.menuName \= g\_windowTitle;
        m\_windowData.size \= g\_windowSize;
        m\_windowData.styles \= WS\_POPUP; // Borderless
          
        // Initialize D3D11 and Win32  
        Init(dxDevice, GuiWndProcHandler);  
    }

    void Render() override {  
        // Force ImGui window to match Win32 window dimensions  
        ImGui::SetNextWindowPos({ 0, 0 });  
        ImGui::SetNextWindowSize({ (float)m\_windowData.size.x, (float)m\_windowData.size.y });

        ImGui::Begin("Dashboard", m\_pIsOpen, ImGuiWindowFlags\_NoTitleBar | ImGuiWindowFlags\_NoResize);  
          
        ImGui::TextColored(IMGUI\_COLORS::skyBlue, "Framework Active");  
          
        // Render persistent components here...

        ImGui::End();  
    }  
};
````

### **3\. The Main Application Loop**

Your entry point should initialize the hardware device, the window, and any custom fonts.

int main() {  
    bool isRunning \= true;  
    DxDevice dxDevice;

    // 1\. Setup screen/window definitions  
    GuiDefinitionsInit();

    // 2\. Setup DX11 Hardware  
    render::CreateD3DDevice(dxDevice);

    // 3\. Create Window Instance  
    MainMenu menu(dxDevice, \&isRunning);

    // 4\. (Optional) Load Custom Fonts  
    // FP\_GUI::InitFonts(ImGui::GetIO(), myFontData);

    // 5\. Execution Loop  
    while (isRunning) {  
        menu.StartRender(); // Polls Win32 messages & ImGui::NewFrame()  
        menu.Render();      // Your overridden logic  
        menu.EndRender();   // ImGui::Render() & d3dContext-\>Present()  
    }

    return 0;  
}

## **🧱 Advanced: Component System**

The framework uses a **Declarative Component Pattern**. Instead of calling ImGui::Button every frame with manual state, wrap them in a GuiElementWrapper.

### **Defining a Widget**

inline const GuiElementWrapper\<FpGuiButton\> primaryBtn \= { ImVec2(150, 40), \[\]() {  
    FpGuiButton b;  
    b.text \= "Confirm";  
    b.backgroundColor \= IMGUI\_COLORS::charcoal;  
    b.rounding \= 8.f;  
    // Link an animation preset  
    b.panim \= \&FP\_GUI\_DEFAULTS::anims::buttons::regular;   
    return b;  
}};

### **Using the Widget**

void Render() {  
    // Retrieve the persistent instance  
    static auto& button \= primaryBtn.Get();  
      
    // Auto-center using geometry helpers  
    g\_guiGeom.CalculateAndSetCenterX(button.size.x);  
      
    if (button.active \= FP\_GUI::Button(button.active, button.size, button.params)) {  
        // Triggered\!  
    }  
}

## **📚 Examples**

For a complete and working implementation, check out the `gui_framework/examples/` directory.

The simple_gui_example demonstrates:

* Full borderless window setup.
* Loading custom fonts from memory.
* Using the GuiElementWrapper for interactive buttons.
* Centering logic using the GuiGeom class utility.

## **🎨 Styling & Utilities**

* **Colors**: Access standardized palettes in gui\_framework\_colors.h (e.g., IMGUI\_COLORS::deepGray).  
* **Fonts**: Use FP\_GUI::Font::GetFont(float size) to retrieve pre-rasterized fonts at specific scales without blurry interpolation.  
* **NotiLabel**: Use FP\_GUI::NotiLabel for temporary, centered status messages that fade out automatically.

## **⚙️ Technical Requirements**

* **Compiler**: C++17 Compatible (MSVC Recommended).  
* **OS**: Windows (utilizes dwmapi.h and d3d11.h).  
* **SDK**: DirectX June 2010 or modern Windows SDK.

**Author**: speedaru