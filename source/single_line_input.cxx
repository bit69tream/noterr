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
  single_line_input::single_line_input(raylib::Rectangle bounding_box, const theme &theme, bool center_text)
      : m_border_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text) {
    m_bounding_box = raylib_helper::subtract_border_from_rectangle(m_border_box, m_theme);
  }

  single_line_input::single_line_input(std::wstring text, raylib::Rectangle bounding_box, const theme &theme, bool center_text)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_center_text(center_text),
        m_text(std::move(text)) {
    m_text_dimensions = raylib_helper::get_line_dimensions(m_text, m_theme);
  }

  bool single_line_input::can_focus(raylib::Vector2 point) const {
    return CheckCollisionPointRec(point, m_bounding_box);
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
      .x = line_dimensions.x + m_bounding_box.x + m_theme.glyph_spacing + m_theme.cursor_width,
      .y = m_bounding_box.y,
      .width = m_theme.cursor_width,
      .height = static_cast<float>(m_theme.font.baseSize),
    };

    DrawRectangleRec(cursor, m_theme.cursor_color);
  }

  void single_line_input::render() const {
    using namespace raylib;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.entity_background);

    render_cursor();

    DrawTextCodepoints(m_theme.font, (const int *)m_text.data(), (int)m_text.size(), Vector2 {m_bounding_box.x, m_bounding_box.y}, m_theme.font_size, m_theme.glyph_spacing, m_theme.entity_foreground);
  }

  void single_line_input::send_events(std::span<event> events) {
    for (const auto &boxed_event : events) {
      if (std::holds_alternative<mouse_event>(boxed_event)) {
        auto event = std::get<mouse_event>(boxed_event);
        std::wcout << L"mouse click event!!!!! (" << event.point.x << L", " << event.point.y << L")" << std::endl;
      } else if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);
        std::wcout << L"key: " << event.codepoint << L"; " << event.key << std::endl;

        // TODO: rewrite this
        if (event.codepoint) {
          m_text.insert(static_cast<size_t>(m_cursor_position), 1, event.codepoint);
          m_cursor_position ++;
        } else if (event.key) {
          switch (event.key) {
          case raylib::KEY_LEFT: {
            m_cursor_position --;
          } break;
          case raylib::KEY_RIGHT: {
            m_cursor_position ++;
          } break;
          case raylib::KEY_BACKSPACE: {
            m_cursor_position --;
            m_text = m_text.erase(static_cast<size_t>(m_cursor_position), 1);
          }
          default: break;
          };
        }
      }
    }

    m_cursor_position = std::clamp(m_cursor_position, static_cast<ssize_t>(0), static_cast<ssize_t>(m_text.size()));
  }
}  // namespace ui
