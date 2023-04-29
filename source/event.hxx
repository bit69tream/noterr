#pragma once

#include <variant>

#include "raylib.hxx"

namespace ui {
  struct mouse_event {
    raylib::Vector2 point;
  };

  struct keyboard_event {
    raylib::KeyboardKey key;
    wchar_t codepoint;
  };

  using event = std::variant<mouse_event, keyboard_event>;
}  // namespace ui
