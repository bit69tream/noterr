#pragma once

#include "raylib.hxx"

namespace ui {
  class object {
  public:
    virtual void render() const = 0;
    virtual bool can_focus(raylib::Vector2 point) const = 0;
    virtual void focus(raylib::Vector2 point) = 0;
    virtual void unfocus() = 0;
  };
};  // namespace ui
