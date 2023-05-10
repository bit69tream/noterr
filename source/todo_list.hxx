#pragma once

#include <string>
#include <vector>
#include <memory>

#include "raylib.hxx"
#include "entity.hxx"
#include "theme.hxx"
#include "single_line_input.hxx"

namespace ui {
  struct task {
    bool is_done;
    std::wstring text;
  };

  class todo_list final : public entity {
  public:
    void render() const override;
    bool can_focus(raylib::Vector2 point) const override;
    void send_events(std::span<event> events) override;

    todo_list(raylib::Rectangle bounding_box, std::wstring title, std::vector<task> tasks, const theme &theme);
    todo_list(raylib::Rectangle bounding_box, const theme &theme);
    ~todo_list() = default;

  private:
    raylib::Rectangle title_delimiter();

  private:
    theme m_theme;

    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;

    bool m_title_focused{false};
    std::shared_ptr<raylib::Rectangle> m_title_bounding_box;
    single_line_input m_title;

    raylib::Rectangle m_title_delimiter;

    std::vector<task> m_tasks;
  };
};  // namespace ui
