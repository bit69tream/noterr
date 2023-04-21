#pragma once

#include <string>

#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class note {
  public:
    void render() const;

    note(raylib::Rectangle box, std::string title, std::string text, theme theme);
    ~note();
  private:
    void compute_element_coordinates();

    raylib::Rectangle m_box, m_border_box;

    raylib::Rectangle m_splitter_line;

    std::string m_title;
    raylib::Vector2 m_title_position;

    std::string m_text;
    raylib::Vector2 m_text_position;

    theme m_theme;
  };
};
