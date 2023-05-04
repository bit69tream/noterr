#pragma once

#include <memory>
#include <span>
#include <string>

#include "entity.hxx"
#include "event.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class single_line_input : public entity {
  public:
    single_line_input(std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme, bool center_text = false);
    single_line_input(std::wstring text, std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme, bool center_text = false);

    void render() const override;
    bool can_focus(raylib::Vector2 point) const override;
    void send_events(std::span<event> events) override;

  private:
    void render_cursor() const;
    void adjust_bounding_box();

  private:
    std::shared_ptr<raylib::Rectangle> m_bounding_box;
    theme m_theme;
    bool m_center_text;
    std::wstring m_text;
    raylib::Vector2 m_text_dimensions;
    bool m_focused {false};
    ssize_t m_cursor_position {0};
  };
}  // namespace ui
