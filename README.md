# 🧩 FP GUI Framework

A lightweight and modern **C++ GUI framework** built on top of [Dear ImGui](https://github.com/ocornut/imgui), designed to simplify UI creation and make it more modular, reusable, and clean.

This framework introduces a **component-based design** that lets you declare UI elements as persistent objects with parameters, colors, animations, and event handlers — all in pure C++.

---

## 📁 Project Structure

```
gui_elements.hpp
gui_framework.cpp
gui_framework.h
gui_framework_colors.h
gui_framework_includes.h
gui_framework_types.hpp
````

- **gui_elements.hpp** — Where you create your gui elements
- **gui_framework.h** — Core logic of the framework (rendering, element handling, utilities)
- **gui_framework_colors.h** — Color definitions and presets
- **gui_framework_includes.h** — All required includes for ImGui + framework setup
- **gui_framework_types.hpp** — Type definitions (structures, templates, wrappers, etc.)

---

## 🚀 Example Usage

Here’s how you can define and use a GUI button using the framework:

```cpp
/* in gui_elements.hpp */
inline const GuiElementWrapper<FpGuiButton> loginButton(ImVec2(150, 35), []() {
    FpGuiButton button;
    button.text = "ok";
    button.textColor = IMGUI_COLORS::white;
    button.backgroundColor = FP_GUI_DEFAULTS::colors::buttonColor;
    button.outline = _FpGuiOutline(FP_GUI_DEFAULTS::colors::defaultOutline, 2.f);
    button.rounding = 6.f;
    button.panim = &FP_GUI_DEFAULTS::anims::buttons::regular;
    return button;
});

/* in your imgui rendering loop */
static GuiElement<FpGuiButton>& loginButton = gui_elements::buttons::loginButton.Get();

if (loginButton.active = FP_GUI::Button(loginButton.active, loginButton.size, loginButton.params)) {
    // your code when button is pressed
}
````

This structure allows for:

* **Declarative element creation** (like React, but in C++)
* **Reusability** — wrap elements in a single line and access them globally
* **Clean separation** between definition, logic, and rendering

---

## 🧱 Features

✅ Easy-to-use wrappers for ImGui elements
🎨 Custom color and animation presets (`FP_GUI_DEFAULTS`)
🧩 Type-safe GUI elements with templated wrappers
🧠 Component-based design for clean architecture
🌀 Built-in animation and outline utilities

---

## ⚙️ Dependencies

* [Dear ImGui](https://github.com/ocornut/imgui)
* C++17 or higher
* (Optional) multithreading support for async actions

---

## 📦 Integration

Just include the framework headers in your project:

```cpp
#include "gui_framework.h"
#include "gui_elements.hpp"
```

Then initialize your GUI elements inside gui_elements.hpp (examples provided), then inside
your ImGui frame loop call the functions from gui_framework.h with your gui elements from gui_elements.hpp.

__OPTIONAL__:
Set your menu's window size in gui_geom.h if your using NotiLabel() so that it centers the label properly.


## 📜 License

MIT License — feel free to use, modify, and distribute this framework.

---

## 👤 Author

**speedaru**
