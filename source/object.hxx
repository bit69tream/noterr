#pragma once

namespace ui {
  class object {
  public:
    virtual void render() const = 0;
    virtual void focus() = 0;
    virtual void unfocus() = 0;
  };
};  // namespace ui
