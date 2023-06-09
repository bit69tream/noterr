#include <optional>
#include <stdexcept>

#include "popup.hxx"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
  popup::popup(std::vector<std::tuple<std::string_view, popup_actions>> entries, raylib::Vector2 position, const theme &theme) : m_theme(theme) {
    using namespace raylib;

    if (entries.size() == 0) {
      throw std::invalid_argument("entries vector must contain at least one element.");
    }

    m_entries.reserve(entries.size());

    float popup_width = 0;
    Vector2 next_position = position;
    for (const auto &entry : entries) {
      std::string_view text = std::get<std::string_view>(entry);
      Vector2 size = MeasureTextEx(m_theme.font, text.data(), m_theme.font_size, m_theme.glyph_spacing);
      popup_width = std::max(size.x, popup_width);

      m_entries.push_back(popup_entry {
        .text = std::get<std::string_view>(entry),
        .action = std::get<popup_actions>(entry),
        .position = next_position,
        .dimensions = size,
      });

      next_position.y += size.y;
    }
    popup_width += m_theme.popup_text_padding * 2;

    m_border = Rectangle {
      .x = position.x - m_theme.border_size,
      .y = position.y - m_theme.border_size,
      .width = popup_width + (m_theme.border_size * 2),
      .height = (next_position.y - position.y) + (m_theme.border_size * 2),
    };

    for (auto &entry : m_entries) {
      entry.dimensions.x = popup_width;
    }
  }

  std::optional<popup_actions> popup::get_action() {
    raylib::Vector2 mouse_position = raylib::GetMousePosition();
    for (const auto &entry : m_entries) {
      raylib::Rectangle entry_box = raylib_helper::from_vectors(entry.position, entry.dimensions);

      if (CheckCollisionPointRec(mouse_position, entry_box)) {
        return entry.action;
      }
    }

    return {};
  }

  void popup::render() {
    using namespace raylib;

    Vector2 mouse_position = GetMousePosition();
    bool isFocusingAnEntry = false;
    DrawRectangleRec(m_border, m_theme.border);

    for (const auto &entry : m_entries) {
      Color popup_background = LIGHTGRAY;
      Color popup_foreground = BLACK;
      Rectangle entry_box = raylib_helper::from_vectors(entry.position, entry.dimensions);

      if (CheckCollisionPointRec(mouse_position, entry_box)) {
        std::swap(popup_background, popup_foreground);
        isFocusingAnEntry = true;
      }

      DrawRectangleRec(entry_box, popup_background);

      Vector2 text_position = entry.position;
      text_position.x += m_theme.popup_text_padding;
      DrawTextEx(m_theme.font, entry.text.data(), text_position, m_theme.font_size, m_theme.glyph_spacing, popup_foreground);
    }

    if (isFocusingAnEntry) {
      SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
      SetMouseCursor(MOUSE_CURSOR_ARROW);
    }
  }

  popup::~popup() = default;
};  // namespace ui
