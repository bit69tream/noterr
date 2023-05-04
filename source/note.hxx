#pragma once

#include <compare>
#include <memory>
#include <string>

#include "entity.hxx"
#include "event.hxx"
#include "raylib.hxx"
#include "single_line_input.hxx"
#include "theme.hxx"

namespace ui {
  class note : public entity {
  public:
    void render() const override;
    bool can_focus(raylib::Vector2 point) const override;
    void send_events(std::span<event> events) override;

    note(raylib::Rectangle bounding_box, std::wstring title, std::wstring text, const theme &theme);
    note(raylib::Rectangle bounding_box, const theme &theme);
    ~note();

  private:
    void compute_element_coordinates();

  private:
    raylib::Rectangle m_bounding_box;
    raylib::Rectangle m_border_box;
    raylib::Rectangle m_title_delimiter;

    std::unique_ptr<single_line_input> m_title;
    std::shared_ptr<raylib::Rectangle> m_title_bounding_box;

    std::wstring m_text;
    raylib::Rectangle m_text_bounding_box;

    theme m_theme;
    bool m_focused {false};
    bool m_title_focused {false};
  };
};  // namespace ui
