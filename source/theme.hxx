#pragma once

#include "raylib.hxx"

namespace ui {
  struct theme {
    float font_size;
    float glyph_spacing;
    float line_spacing;
    raylib::Font font;

    bool render_text_cursor_only_on_mouse_hover;
    bool render_text_cursor_only_on_focus;

    raylib::Color background;

    raylib::Color grid_color;
    float grid_tile_size_as_percentage;

    raylib::Color border;
    float border_size;

    raylib::Color popup_background;
    raylib::Color popup_foreground;
    float popup_text_padding;

    raylib::Color placeholder;

    raylib::Color entity_background;
    raylib::Color entity_foreground;

    float cursor_width;
    raylib::Color cursor_color;
  };
};  // namespace ui
