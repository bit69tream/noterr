#pragma once

#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  enum class popup_actions {
    create_new_card,
    create_new_note,
    restore_zoom,
    quit,
  };

  struct popup_entry {
    std::string_view text;
    popup_actions action;
    raylib::Vector2 position, dimensions;
  };

  class popup {
  public:
    popup(std::vector<std::tuple<std::string_view, popup_actions>> entries,
          raylib::Vector2 position, const theme &theme);
    void render();
    std::optional<popup_actions> get_action();
    ~popup();

  private:
    std::vector<popup_entry> m_entries;
    theme m_theme;
    raylib::Rectangle m_border;
  };
};  // namespace ui
