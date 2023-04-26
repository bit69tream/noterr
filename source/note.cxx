#include <string_view>

#include "note.hxx"
#include "raylib.h"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  // TODO: minimal placeholder and note size
  note::note(raylib::Rectangle box, std::wstring title, std::wstring text,
             theme theme)
      : m_box(box),
        m_border_box(box),
        m_title(std::move(title)),
        m_text(std::move(text)),
        m_theme(theme) {
    compute_element_coordinates();
  }

  note::note(raylib::Rectangle box, theme theme)
      : m_box(box),
        m_border_box(box),
        m_theme(theme) {
    compute_element_coordinates();
  }

  note::~note() {
  }

  bool note::operator>(const note &that) const {
    return (this->m_border_box.width > that.m_border_box.width) && (this->m_border_box.height > that.m_border_box.height);
  }

  void note::compute_element_coordinates() {
    using namespace raylib;

    m_box.x += m_theme.border_size;
    m_box.y += m_theme.border_size;
    m_box.width -= m_theme.border_size * 2;
    m_box.height -= m_theme.border_size * 2;

    float title_text_height = 0;
    for (auto codepoint : m_title) {
      const int glyph_index = GetGlyphIndex(m_theme.font, codepoint);
      const Rectangle &glyph_box = m_theme.font.recs[glyph_index];
      const float glyph_height = (glyph_box.height);

      title_text_height = std::max(glyph_height, title_text_height);
    }

    m_title_bounding_box = Rectangle {
      .x = m_box.x + m_theme.font_spacing,
      .y = m_box.y + m_theme.font_spacing,
      .width = m_box.width - (2 * m_theme.font_spacing),
      .height = title_text_height + m_theme.font_spacing,
    };

    m_title_delimiter = Rectangle {
      .x = m_box.x + m_theme.border_size,
      .y = m_title_bounding_box.y + m_title_bounding_box.height,
      .width = m_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };

    m_text_bounding_box = Rectangle {
      .x = m_box.x + m_theme.font_spacing,
      .y = m_title_delimiter.y + m_title_delimiter.height + m_theme.font_spacing,
      .width = m_box.width - (2 * m_theme.font_spacing),
      .height = m_box.height - m_title_bounding_box.height - (4 * m_theme.font_spacing) - m_title_delimiter.height};
  }

  // TODO: scrolling
  // TODO: title should be rendered differently
  void note::render() const {
    using namespace raylib;
    using namespace raylib_helper;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_box, m_theme.note_background);
    DrawRectangleRec(m_title_delimiter, m_theme.border);

    render_wrapping_text_in_bounds(m_title, m_title_bounding_box, m_theme);
    render_wrapping_text_in_bounds(m_text, m_text_bounding_box, m_theme);
  }
};  // namespace ui
