#pragma once

#include <memory>
#include <string>
#include <vector>

#include "entity.hxx"
#include "raylib.hxx"
#include "single_line_input.hxx"
#include "theme.hxx"

namespace ui {
  struct task {
    bool is_done;
    std::wstring text;
  };

  class todo_list final : public entity {
  public:
    void render(raylib::Vector2 mouse_position_in_the_world, bool focused) const override;
    bool can_focus(raylib::Vector2 point) const override;
    void send_events(std::span<event> events) override;

    todo_list(raylib::Rectangle bounding_box, std::wstring title, std::vector<task> tasks, const theme &theme);
    todo_list(raylib::Rectangle bounding_box, const theme &theme);
    ~todo_list() = default;

  private:
    raylib::Rectangle title_delimiter();
    void compute_element_coordinates();

  private:
    theme m_theme;

    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;

    bool m_title_focused {true};
    std::shared_ptr<raylib::Rectangle> m_title_bounding_box;
    single_line_input m_title;

    raylib::Rectangle m_title_delimiter;

    std::vector<task> m_tasks;
  };
};  // namespace ui
