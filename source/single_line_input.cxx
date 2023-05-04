#include <algorithm>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string_view>

#include "raylib.hxx"
#include "raylib_helper.hxx"
#include "single_line_input.hxx"

namespace ui {
  single_line_input::single_line_input(std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text) {
  }

  single_line_input::single_line_input(std::wstring text, std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text),
        m_text(std::move(text)) {
    m_text_dimensions = raylib_helper::get_line_dimensions(m_text, m_theme);
  }

  void single_line_input::render_cursor() const {
    using namespace raylib;

    std::wstring_view text_before_cursor = m_text;
    text_before_cursor = text_before_cursor.substr(0, static_cast<size_t>(m_cursor_position));

    if (text_before_cursor.empty()) {
      return;
    }

    const Vector2 line_dimensions = raylib_helper::get_line_dimensions(text_before_cursor, m_theme);
    const Rectangle cursor = Rectangle {
      .x = line_dimensions.x + m_bounding_box->x,
      .y = m_bounding_box->y,
      .width = m_theme.cursor_width,
      .height = static_cast<float>(m_theme.font.baseSize),
    };

    DrawRectangleRec(cursor, m_theme.cursor_color);
  }

  void single_line_input::adjust_bounding_box() {
    m_bounding_box->width = m_text_dimensions.x;
    m_bounding_box->height = m_text_dimensions.y;
  }

  void single_line_input::render() const {
    using namespace raylib;

    render_cursor();
    DrawTextCodepoints(m_theme.font, reinterpret_cast<const int *>(m_text.data()), static_cast<int>(m_text.size()), Vector2 {m_bounding_box->x, m_bounding_box->y}, m_theme.font_size, m_theme.glyph_spacing, m_theme.entity_foreground);
  }

  void single_line_input::send_events(std::span<event> events) {
    for (const auto &boxed_event : events) {
      if (std::holds_alternative<mouse_event>(boxed_event)) {
        auto event = std::get<mouse_event>(boxed_event);
        const int point_x_offset = static_cast<int>(event.point.x - m_bounding_box->x);

        m_cursor_position = static_cast<ssize_t>(m_text.size());
        int text_width = 0;
        for (size_t i = 0; i < m_text.size(); i++) {
          const int glyph_index = raylib::GetGlyphIndex(m_theme.font, m_text[i]);
          const auto &glyph = m_theme.font.glyphs[glyph_index];

          text_width += glyph.advanceX + static_cast<int>(m_theme.glyph_spacing);
          if (text_width > point_x_offset) {
            m_cursor_position = static_cast<ssize_t>(i);
            break;
          }
        }
      } else if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);

        if (event.codepoint) {
          m_text.insert(static_cast<size_t>(m_cursor_position), 1, event.codepoint);
          m_cursor_position++;
        } else if (event.key) {
          switch (event.key) {
            case raylib::KEY_LEFT: {
              m_cursor_position--;
            } break;
            case raylib::KEY_RIGHT: {
              m_cursor_position++;
            } break;
            case raylib::KEY_BACKSPACE: {
              m_cursor_position--;
              m_text = m_text.erase(static_cast<size_t>(m_cursor_position), 1);
            }
            default: break;
          };
        }
      }
    }

    m_cursor_position = std::clamp(m_cursor_position, static_cast<ssize_t>(0), static_cast<ssize_t>(m_text.size()));
    m_text_dimensions = raylib_helper::get_line_dimensions(m_text, m_theme);
    adjust_bounding_box();
  }
}  // namespace ui
