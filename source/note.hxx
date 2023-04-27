#pragma once

#include <compare>
#include <string>

#include "object.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class note : public object {
  public:
    void render() const override;
    void focus() override;
    void unfocus() override;

    note(raylib::Rectangle box, std::wstring title, std::wstring text, theme theme);
    note(raylib::Rectangle box, theme theme);
    ~note();

  private:
    void compute_element_coordinates();

    raylib::Rectangle m_box, m_border_box;

    raylib::Rectangle m_title_delimiter;

  private:
    std::wstring m_title;
    raylib::Rectangle m_title_bounding_box;

    std::wstring m_text;
    raylib::Rectangle m_text_bounding_box;

    theme m_theme;
    bool m_focused;
  };
};  // namespace ui
