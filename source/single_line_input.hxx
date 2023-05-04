#pragma once

#include <memory>
#include <span>
#include <string>

#include "element.hxx"
#include "event.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class single_line_input : public element {
  public:
    single_line_input(std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme);
    single_line_input(std::wstring text, std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme);

    ~single_line_input() = default;

    void render() const override;
    void send_events(std::span<event> events) override;

  private:
    void render_cursor() const;
    void adjust_bounding_box();

  private:
    std::shared_ptr<raylib::Rectangle> m_bounding_box;
    theme m_theme;
    std::wstring m_text;
    raylib::Vector2 m_text_dimensions;
    raylib::Vector2 m_text_position;
    bool m_focused {false};
    ssize_t m_cursor_position {0};
  };
}  // namespace ui
