#include <string_view>

#include "note.hxx"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  // TODO: minimal placeholder and note size
  note::note(raylib::Rectangle box, std::string title, std::string text,
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

  note::~note() {}

  bool note::operator>(const note &that) const {
    return (this->m_border_box.width > that.m_border_box.width)
      && (this->m_border_box.height > that.m_border_box.height);
  }

  void note::compute_element_coordinates() {
    using namespace raylib;

    m_box.x += m_theme.border_size;
    m_box.y += m_theme.border_size;
    m_box.width -= m_theme.border_size * 2;
    m_box.height -= m_theme.border_size * 2;

    const char *temp;
    if (m_title.empty()) {
      temp = " ";
    } else {
      temp = m_title.c_str();
    }
    Vector2 title_size = MeasureTextEx(m_theme.font, temp,
                                       m_theme.font_size, m_theme.font_spacing);

    m_splitter_line = Rectangle {
      .x = m_box.x + m_theme.border_size,
      .y = m_box.y + title_size.y + (2 * m_theme.font_spacing),
      .width = m_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };

    m_title_position = Vector2 {
      .x = m_box.x + (m_box.width / 2) - (title_size.x / 2),
      .y = m_box.y + m_theme.font_spacing,
    };

    m_text_bounding_box = Rectangle {
      .x = m_box.x + m_theme.font_spacing,
      .y = m_splitter_line.y + m_splitter_line.height + m_theme.font_spacing,
      .width = m_box.width - (2 * m_theme.font_spacing),
      .height = m_box.height - title_size.y - (4 * m_theme.font_spacing) - m_theme.border_size,
    };
  }

  // TODO: scrolling
  // TODO: do something when title is too large to fit in the box
  void note::render() const {
    using namespace raylib;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_box, m_theme.note_background);

    DrawTextEx(m_theme.font, m_title.c_str(), m_title_position, m_theme.font_size,
               m_theme.font_spacing, m_theme.note_foreground);
    DrawRectangleRec(m_splitter_line, m_theme.border);

    raylib_helper::render_wrapping_text_in_bounds(m_text, m_text_bounding_box, m_theme);
  }
};  // namespace ui
