#pragma once

#include <span>

#include "event.hxx"

namespace ui {
  class element {
  public:
    virtual void render() const = 0;
    virtual void send_events(std::span<event> events) = 0;
  };
};  // namespace ui
