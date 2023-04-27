#include "single_line_input.hxx"
#include "raylib.hxx"
#include <algorithm>
#include <memory>

namespace ui {
  single_line_input::single_line_input(raylib::Rectangle bounding_box, theme theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text) {
    m_bounding_box.x += m_theme.border_size;
    m_bounding_box.y += m_theme.border_size;
    m_bounding_box.width -= m_theme.border_size * 2;
    m_bounding_box.height -= m_theme.border_size * 2;
  }


  single_line_input::single_line_input(std::wstring text, raylib::Rectangle bounding_box, theme theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text),
        m_text(std::move(text)) {
    calculate_text_dimensions();
  }


  single_line_input::~single_line_input() {
  }

  void single_line_input::calculate_text_dimensions() {
    float width = 0;
    float max_glyph_hight = 0;

    for (auto codepoint : m_text) {
      int glyph_index = raylib::GetGlyphIndex(m_theme.font, codepoint);
      auto &glyph_rectangle = m_theme.font.recs[glyph_index];
      width += glyph_rectangle.width + m_theme.font_spacing;
      max_glyph_hight = std::max(max_glyph_hight, glyph_rectangle.height);
    }
    width -= m_theme.font_spacing;

    m_text_dimensions.x = width;
    m_text_dimensions.y = max_glyph_hight;
  }

  void single_line_input::focus() {
    m_focused = false;
  }

  void single_line_input::unfocus() {
    m_focused = true;
  }

  void single_line_input::render() const {
    using namespace raylib;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.object_background);
  }
}  // namespace ui
