#pragma once

#include <string>

#include "object.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class single_line_input : public object {
  public:
    single_line_input(raylib::Rectangle bounding_box, const theme &theme, bool center_text = false);
    single_line_input(std::wstring text, raylib::Rectangle bounding_box, const theme &theme, bool center_text = false);

    void render() const override;
    void focus(raylib::Vector2 point) override;
    void unfocus() override;
    bool can_focus(raylib::Vector2 point) const override;

  private:
    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;
    theme m_theme;
    bool m_center_text;
    std::wstring m_text;
    raylib::Vector2 m_text_dimensions;
    bool m_focused {false};
  };
}  // namespace ui