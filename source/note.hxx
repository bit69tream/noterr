#pragma once

#include <compare>
#include <string>

#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class note {
    public:
    void render() const;

    note(raylib::Rectangle box, std::string title, std::string text, theme theme);
    note(raylib::Rectangle box, theme theme);
    ~note();

    bool operator>(const note &that) const;

    private:
    void compute_element_coordinates();

    raylib::Rectangle m_box, m_border_box;

    raylib::Rectangle m_splitter_line;

    std::string m_title;
    raylib::Vector2 m_title_position;

    std::string m_text;
    raylib::Rectangle m_text_bounding_box;

    theme m_theme;
  };
};  // namespace ui
