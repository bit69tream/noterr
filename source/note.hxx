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
    raylib::Rectangle m_box, m_border_box;
    std::string m_title;
    std::string m_text;
    theme m_theme;
  };
};
