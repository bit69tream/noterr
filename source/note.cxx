#include "note.hxx"
#include "raylib.hxx"

namespace ui {
note::note(raylib::Rectangle box, std::string title, std::string text,
           theme theme)
    : m_box(box), m_border_box(box), m_title(std::move(title)),
      m_text(std::move(text)), m_theme(theme) {
  m_box.x += m_theme.border_size;
  m_box.y += m_theme.border_size;
  m_box.width -= m_theme.border_size * 2;
  m_box.height -= m_theme.border_size * 2;

  compute_element_coordinates();
}

note::~note() {}

void note::compute_element_coordinates() {
  using namespace raylib;

  Vector2 title_size = MeasureTextEx(m_theme.font, m_title.c_str(),
                                     m_theme.font_size, m_theme.font_spacing);

  m_splitter_line = Rectangle{
      .x = m_box.x + m_theme.border_size,
      .y = m_box.y + title_size.y + m_theme.font_spacing,
      .width = m_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
  };

  m_title_position = raylib::Vector2{
      .x = m_box.x + (m_box.width / 2) - (title_size.x / 2),
      .y = m_box.y,
  };

  m_text_position = raylib::Vector2{
      .x = m_box.x + m_theme.font_spacing,
      .y = m_splitter_line.y + m_splitter_line.height + m_theme.font_spacing,
  };
}

// TODO: make some way to wrap text
// TODO: do something when title is too large to fit in the box
void note::render() const {
  using namespace raylib;

  DrawRectangleRec(m_border_box, m_theme.border);
  DrawRectangleRec(m_box, m_theme.note_background);

  DrawTextEx(m_theme.font, m_title.c_str(), m_title_position, m_theme.font_size,
             m_theme.font_spacing, m_theme.note_foreground);
  DrawRectangleRec(m_splitter_line, m_theme.border);

  DrawTextEx(m_theme.font, m_text.c_str(), m_text_position, m_theme.font_size,
             m_theme.font_spacing, m_theme.note_foreground);
}
}; // namespace ui