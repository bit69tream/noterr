#include <iostream>
#include <span>
#include <stdexcept>
#include <string_view>

#include "note.hxx"
#include "raylib.h"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  // TODO: minimal placeholder and note size
  note::note(raylib::Rectangle bounding_box, std::wstring title, std::wstring text, const theme &theme)
      : m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_title(std::move(title)),
        m_text(std::move(text)),
        m_theme(theme) {
    compute_element_coordinates();
  }

  note::note(raylib::Rectangle bounding_box, const theme &theme)
      : m_bounding_box(bounding_box),
        m_border_box(bounding_box),
        m_theme(theme) {
    compute_element_coordinates();
  }

  note::~note() {
  }

  void note::compute_element_coordinates() {
    using namespace raylib;

    m_bounding_box = raylib_helper::subtract_border_from_rectangle(m_border_box, m_theme);

    float title_text_height = 0;
    for (auto codepoint : m_title) {
      const int glyph_index = GetGlyphIndex(m_theme.font, codepoint);
      const Rectangle &glyph_box = m_theme.font.recs[glyph_index];
      const float glyph_height = (glyph_box.height);

      title_text_height = std::max(glyph_height, title_text_height);
    }

    m_title_bounding_box = Rectangle {
      .x = m_bounding_box.x + m_theme.glyph_spacing,
      .y = m_bounding_box.y + m_theme.glyph_spacing,
      .width = m_bounding_box.width - (2 * m_theme.glyph_spacing),
      .height = title_text_height + m_theme.glyph_spacing,
    };

    m_title_delimiter = Rectangle {
      .x = m_bounding_box.x + m_theme.border_size,
      .y = m_title_bounding_box.y + m_title_bounding_box.height,
      .width = m_bounding_box.width - (m_theme.border_size * 2),
      .height = m_theme.border_size,
    };

    m_text_bounding_box = Rectangle {
      .x = m_bounding_box.x + m_theme.glyph_spacing,
      .y = m_title_delimiter.y + m_title_delimiter.height + m_theme.glyph_spacing,
      .width = m_bounding_box.width - (2 * m_theme.glyph_spacing),
      .height = m_bounding_box.height - m_title_bounding_box.height - (4 * m_theme.glyph_spacing) - m_title_delimiter.height,
    };
  }

  bool note::can_focus(raylib::Vector2 point) const {
    return CheckCollisionPointRec(point, m_bounding_box);
  }

  void note::send_events(std::span<event> events) {
    for (const auto &event : events) {
      if (std::holds_alternative<mouse_click_event>(event)) {
        auto mouse_event = std::get<mouse_click_event>(event);
        std::wcout << L"mouse click event!!!!! (" << mouse_event.point.x << L", " << mouse_event.point.y << L")" << std::endl;
      } else if (std::holds_alternative<keyboard_key_press_event>(event)) {
        auto keyboard_event = std::get<keyboard_key_press_event>(event);
        std::wcout << L"key: " << keyboard_event.codepoint << std::endl;
      }
    }
  }

  // TODO: scrolling
  // TODO: title should be rendered differently
  void note::render() const {
    using namespace raylib;
    using namespace raylib_helper;

    DrawRectangleRec(m_border_box, m_theme.border);
    DrawRectangleRec(m_bounding_box, m_theme.object_background);
    DrawRectangleRec(m_title_delimiter, m_theme.border);

    render_wrapping_text_in_bounds(m_title, m_title_bounding_box, m_theme);
    render_wrapping_text_in_bounds(m_text, m_text_bounding_box, m_theme);
  }
};  // namespace ui
