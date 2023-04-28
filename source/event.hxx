#pragma once

#include <variant>

#include "raylib.hxx"

namespace ui {
  struct mouse_click_event {
    raylib::Vector2 point;
  };

  struct keyboard_key_press_event {
    raylib::KeyboardKey key;
    wchar_t codepoint;
  };

  using event = std::variant<mouse_click_event, keyboard_key_press_event>;
}  // namespace ui
