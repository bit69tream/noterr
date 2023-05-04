#pragma once

#include <vector>
#include <span>
#include <string>

#include "raylib.hxx"
#include "element.hxx"

namespace ui {
  class multi_line_input : public element {
  public:

    multi_line_input();
    ~multi_line_input();

    void render() const override;
    void send_events(std::span<event> events) override;

  private:
    std::vector<std::wstring> m_lines;
    size_t m_cursor_row;
    size_t m_cursor_column;
  };
};
