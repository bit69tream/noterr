#include <algorithm>
#include <iostream>

#include "raylib_helper.hxx"
#include "theme.hxx"

namespace raylib_helper {
  Rectangle from_vectors(Vector2 position, Vector2 dimensions) {
    return Rectangle {
      .x = position.x,
      .y = position.y,
      .width = dimensions.x,
      .height = dimensions.y,
    };
  }

  Rectangle into_a_rectangle_where_the_top_left_corder_is_actually_a_top_left_corner(Rectangle rectangle) {
    if (rectangle.width < 0) {
      rectangle.width = -rectangle.width;
      rectangle.x -= rectangle.width;
    }

    if (rectangle.height < 0) {
      rectangle.height = -rectangle.height;
      rectangle.y -= rectangle.height;
    }

    return rectangle;
  }

  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle, Camera2D camera) {
    Vector2 top_left = {.x = rectangle.x, .y = rectangle.y};
    Vector2 world_top_left = GetScreenToWorld2D(top_left, camera);

    rectangle.x = world_top_left.x;
    rectangle.y = world_top_left.y;
    rectangle.width /= camera.zoom;
    rectangle.height /= camera.zoom;

    return rectangle;
  }

  Rectangle subtract_border_from_rectangle(Rectangle rectangle, const ui::theme &theme) {
    rectangle.x += theme.border_size;
    rectangle.y += theme.border_size;
    rectangle.width -= theme.border_size * 2;
    rectangle.height -= theme.border_size * 2;

    return rectangle;
  }

  Rectangle add_border_to_rectangle(Rectangle rectangle, const ui::theme &theme) {
    rectangle.x -= theme.border_size;
    rectangle.y -= theme.border_size;
    rectangle.width += theme.border_size * 2;
    rectangle.height += theme.border_size * 2;

    return rectangle;
  }

  Vector2 get_line_dimensions(std::wstring_view line, const ui::theme &theme) {
    float width = 0;
    float height = 0;

    for (const auto &codepoint : line) {
      const int glyph_index = raylib::GetGlyphIndex(theme.font, codepoint);
      const auto &glyph = theme.font.glyphs[glyph_index];
      const auto &glyph_rectangle = theme.font.recs[glyph_index];
      width += static_cast<float>(glyph.advanceX) + theme.glyph_spacing;
      height = std::max(height, glyph_rectangle.height);
    }
    width -= theme.glyph_spacing;

    return {.x = width + (theme.glyph_spacing * 2), .y = height + (theme.line_spacing * 2)};
  }
};  // namespace raylib_helper
