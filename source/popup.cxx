#include <stdexcept>
#include <optional>

#include "popup.hxx"
#include "raylib.hxx"
#include "raylib_helper.hxx"

namespace ui {
    popup::popup(std::vector<std::tuple<std::string_view, popup_actions>> entries, raylib::Vector2 position, theme theme)
    : m_theme(theme) {
      using namespace raylib;

      if (entries.size() == 0) {
        throw std::invalid_argument("entries vector must contain at least one element.");
      }

      m_entries.reserve(entries.size());

      float popup_width = 0;
      Vector2 next_position = position;
      for (const auto &entry : entries) {
        std::string_view text = std::get<std::string_view>(entry);
        Vector2 size = MeasureTextEx(m_theme.font, text.data(), m_theme.font_size, m_theme.font_spacing);
        popup_width = std::max(size.x, popup_width);

        m_entries.push_back(popup_entry{
          .text = std::get<std::string_view>(entry),
          .action = std::get<popup_actions>(entry),
          .position = next_position,
          .dimensions = size,
        });

        next_position.y += size.y;
      }
      popup_width += m_theme.popup_text_padding * 2;

      for (auto &entry : m_entries) {
        entry.dimensions.x = popup_width;
      }
    }

  std::optional<popup_actions> popup::get_action() {
    raylib::Vector2 mouse_position = raylib::GetMousePosition();
    for (const auto &entry : m_entries) {
      raylib::Rectangle entry_box = raylib_helper::RectangleFromVectors(entry.position, entry.dimensions);

      if (CheckCollisionPointRec(mouse_position, entry_box)) {
        return entry.action;
      }
    }

    return {};
  }

  void popup::render() {
    using namespace raylib;

    Vector2 mouse_position = GetMousePosition();
    for (const auto &entry : m_entries) {
      Color popup_background_color = LIGHTGRAY;
      Color popup_foreground_color = BLACK;
      Rectangle entry_box = raylib_helper::RectangleFromVectors(entry.position, entry.dimensions);

      if (CheckCollisionPointRec(mouse_position, entry_box)) {
        std::swap(popup_background_color, popup_foreground_color);
      }

      DrawRectangleRec(entry_box, popup_background_color);

      Vector2 text_position = entry.position;
      text_position.x += m_theme.popup_text_padding;
      DrawTextEx(m_theme.font, entry.text.data(), text_position, m_theme.font_size, m_theme.font_spacing, popup_foreground_color);
    }
  }

  popup::~popup() = default;
};
