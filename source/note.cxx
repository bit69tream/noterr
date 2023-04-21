#include "note.hxx"
#include "raylib.hxx"

namespace ui {
  note::note(raylib::Rectangle box, std::string title, std::string text, theme theme)
    : m_box(box), m_title(title), m_text(text), m_theme(theme) {
    m_border_box = m_box;
    m_box.x += m_theme.border_size;
    m_box.y += m_theme.border_size;
    m_box.width -= m_theme.border_size * 2;
    m_box.height -= m_theme.border_size * 2;
  }

  note::~note() {
  }

  void note::render() const {
    raylib::DrawRectangleRec(m_border_box, m_theme.border);
    raylib::DrawRectangleRec(m_box, m_theme.note_background);
  }
};
