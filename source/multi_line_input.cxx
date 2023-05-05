#include <algorithm>
#include <stdexcept>
#include <string_view>
#include <variant>

#include "event.hxx"
#include "multi_line_input.hxx"
#include "raylib.h"
#include "raylib_helper.hxx"

namespace ui {
  multi_line_input::multi_line_input(std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme)
      : m_bounding_box(bounding_box),
        m_theme(theme) {
    m_lines.push_back(L"");
    calculate_line_dimensions();
    adjust_bounding_box();
  }

  multi_line_input::multi_line_input(std::vector<std::wstring> lines, std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme)
      : m_bounding_box(bounding_box),
        m_theme(theme),
        m_lines(std::move(lines)) {
    calculate_line_dimensions();
    adjust_bounding_box();
  }

  void multi_line_input::adjust_bounding_box() {
    float max_line_width = 0;
    for (const auto &line : m_line_dimensions) {
      max_line_width = std::max(max_line_width, line.x);
    }

    m_bounding_box->width = std::max(m_bounding_box->width, max_line_width);

    float lines_height = m_theme.line_spacing;
    for (const auto &line : m_line_dimensions) {
      lines_height += line.y + m_theme.line_spacing;
    }
    lines_height -= m_theme.line_spacing;

    m_bounding_box->height = std::max(m_bounding_box->height, lines_height);
  }

  void multi_line_input::calculate_line_dimensions() {
    m_line_dimensions.clear();
    m_line_dimensions.resize(m_lines.size());

    for (size_t i = 0; i < m_lines.size(); i++) {
      m_line_dimensions[i] = raylib_helper::get_line_dimensions(m_lines[i], m_theme);
    }
  }

  void multi_line_input::render_cursor() const {
    using namespace raylib;

    const size_t cursor_column = static_cast<size_t>(m_cursor_column);
    const size_t cursor_row = static_cast<size_t>(m_cursor_row);

    std::wstring_view text_before_cursor = m_lines[cursor_row];
    text_before_cursor = text_before_cursor.substr(0, cursor_column);

    if (text_before_cursor.empty()) {
      return;
    }

    float y = m_bounding_box->y;
    for (size_t i = 0; i < cursor_row; i++) {
      y += m_line_dimensions[i].y;
    }

    const Vector2 text_before_cursor_dimensions = raylib_helper::get_line_dimensions(text_before_cursor, m_theme);
    const Rectangle cursor = Rectangle {
      .x = text_before_cursor_dimensions.x + m_bounding_box->x,
      .y = y,
      .width = m_theme.cursor_width,
      .height = static_cast<float>(m_theme.font.baseSize),
    };

    DrawRectangleRec(cursor, m_theme.cursor_color);
  }

  void multi_line_input::render() const {
    if (m_lines.size() != m_line_dimensions.size()) {
      throw std::logic_error("m_lines.size() != m_line_dimensions.size()");
    }

    float y = m_bounding_box->y + m_theme.line_spacing;
    for (size_t i = 0; i < m_lines.size(); i++) {
      raylib::DrawTextCodepoints(m_theme.font, reinterpret_cast<const int *>(m_lines[i].data()), static_cast<int>(m_lines[i].size()), raylib::Vector2 {m_bounding_box->x, y}, m_theme.font_size, m_theme.glyph_spacing, m_theme.entity_foreground);
      y += m_line_dimensions[i].y + m_theme.line_spacing;
    }

    render_cursor();
  }

  void multi_line_input::send_events(std::span<event> events) {
    for (const auto &boxed_event : events) {
      if (std::holds_alternative<keyboard_event>(boxed_event)) {
        auto event = std::get<keyboard_event>(boxed_event);

        if (event.codepoint) {
          m_lines[static_cast<size_t>(m_cursor_row)].insert(static_cast<size_t>(m_cursor_column), 1, event.codepoint);
          m_cursor_column++;
        } else if (event.key) {
          switch (event.key) {
            case raylib::KEY_LEFT: {
              m_cursor_column--;
            } break;
            case raylib::KEY_RIGHT: {
              m_cursor_column++;
            } break;
            case raylib::KEY_UP: {
              m_cursor_row--;
            } break;
            case raylib::KEY_DOWN: {
              m_cursor_row++;
            } break;
            case raylib::KEY_ENTER: {
              m_lines.push_back(m_lines[static_cast<size_t>(m_cursor_row)].substr(static_cast<size_t>(m_cursor_column)));
              m_lines[static_cast<size_t>(m_cursor_row)].erase(static_cast<size_t>(m_cursor_column));
              m_cursor_row++;
              m_cursor_column = 0;
            } break;
            case raylib::KEY_BACKSPACE: {
              m_cursor_column--;
              if (m_cursor_column < 0) {
                break;
              }

              m_lines[static_cast<size_t>(m_cursor_row)] = m_lines[static_cast<size_t>(m_cursor_row)].erase(static_cast<size_t>(m_cursor_column), 1);
            } break;
            default: break;
          }
        }
      }
    }

    m_cursor_row = std::clamp(m_cursor_row, 0L, static_cast<ssize_t>(m_lines.size()));
    m_cursor_column = std::clamp(m_cursor_column, 0L, static_cast<ssize_t>(m_lines[static_cast<size_t>(m_cursor_row)].size()));
    calculate_line_dimensions();
    adjust_bounding_box();
  }
}  // namespace ui
