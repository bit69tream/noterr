#pragma once

#include "raylib.hxx"

namespace ui {
  struct theme {
    float font_size;
    float font_spacing;
    raylib::Font font;

    raylib::Color background_color;

    raylib::Color popup_background_color;
    raylib::Color popup_foreground_color;
    float popup_text_padding;

    raylib::Color placeholder_color;
  };
};
