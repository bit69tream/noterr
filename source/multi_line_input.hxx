#pragma once

#include <memory>
#include <span>
#include <string>
#include <vector>

#include "element.hxx"
#include "raylib.hxx"
#include "theme.hxx"

namespace ui {
  class multi_line_input final : public element {
  public:
    multi_line_input(std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme);
    multi_line_input(std::vector<std::wstring> lines, std::shared_ptr<raylib::Rectangle> bounding_box, const theme &theme);
    ~multi_line_input() = default;

    void render() const override;
    void send_events(std::span<event> events) override;

  private:
    void render_cursor() const;
    void calculate_line_dimensions();
    void adjust_bounding_box();

  private:
    std::shared_ptr<raylib::Rectangle> m_bounding_box;
    theme m_theme;
    std::vector<std::wstring> m_lines;
    std::vector<raylib::Vector2> m_line_dimensions;
    ssize_t m_cursor_row {0};
    ssize_t m_cursor_column {0};
  };
};  // namespace ui
