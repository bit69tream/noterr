#pragma once

#include <string>

#include "object.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class single_line_input : public object {
  public:
    single_line_input(raylib::Rectangle bounding_box, theme theme, bool center_text = false);
    single_line_input(std::wstring text, raylib::Rectangle bounding_box, theme theme, bool center_text = false);

    ~single_line_input();

    void render() const override;
    void focus() override;
    void unfocus() override;

  private:
    void calculate_text_dimensions();

  private:
    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;
    theme m_theme;
    bool m_center_text;
    std::wstring m_text;
    raylib::Vector2 m_text_dimensions;
    bool m_focused;
  };
}  // namespace ui
