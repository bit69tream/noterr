#pragma once

#include <span>

#include "event.hxx"

namespace ui {
  class element {
  public:
    virtual void render(raylib::Vector2 mouse_position_in_the_world) const = 0;
    virtual void send_events(std::span<event> events) = 0;
  };
};  // namespace ui
