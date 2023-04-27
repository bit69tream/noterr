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

  Rectangle into_proper_rectangle(Rectangle rectangle) {
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

  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle,
                                                 Camera2D camera) {
    Vector2 top_left = {.x = rectangle.x, .y = rectangle.y};
    Vector2 world_top_left = GetScreenToWorld2D(top_left, camera);

    rectangle.x = world_top_left.x;
    rectangle.y = world_top_left.y;
    rectangle.width /= camera.zoom;
    rectangle.height /= camera.zoom;

    return rectangle;
  }

  // NOTE: this is slow af
  void render_wrapping_text_in_bounds(std::wstring_view text, Rectangle bounding_box, const ui::theme &theme) {
    const float initial_x_position = bounding_box.x + theme.glyph_spacing;

    float current_line_max_glyph_height = 0;
    Vector2 next_position = {.x = initial_x_position, .y = bounding_box.y};
    for (const auto codepoint : text) {
      const int glyph_index = GetGlyphIndex(theme.font, codepoint);
      const Rectangle &glyph_box = theme.font.recs[glyph_index];

      current_line_max_glyph_height = std::max(glyph_box.height, current_line_max_glyph_height);

      if (codepoint == '\n') {
        next_position.x = initial_x_position;
        next_position.y += current_line_max_glyph_height;
        current_line_max_glyph_height = static_cast<float>(theme.font.baseSize) / 2.0f;
        continue;
      }

      Vector2 current_position = next_position;
      next_position.x += glyph_box.width + theme.glyph_spacing;

      bool is_inside_of_bounding_box = CheckCollisionPointRec(next_position, bounding_box);
      if (!is_inside_of_bounding_box) {
        current_position.x = initial_x_position;
        current_position.y += current_line_max_glyph_height;
        current_line_max_glyph_height = glyph_box.height;

        next_position = current_position;
        next_position.x += glyph_box.width + theme.glyph_spacing;
      }

      bool cannot_be_put_inside_bounding_box = (current_position.y + glyph_box.height) >= (bounding_box.y + bounding_box.height);
      if (cannot_be_put_inside_bounding_box) {
        break;
      }

      DrawTextCodepoint(theme.font, codepoint, current_position, theme.font_size, theme.object_foreground);
    }
  }

  Rectangle subtract_border_from_rectangle(Rectangle rectangle, const ui::theme &theme) {
    rectangle.x += theme.border_size;
    rectangle.y += theme.border_size;
    rectangle.width -= theme.border_size * 2;
    rectangle.height -= theme.border_size * 2;

    return rectangle;
  }

  Vector2 get_line_dimensions(std::wstring_view line, const ui::theme &theme) {
    float width = 0;
    float height = 0;

    for (const auto &codepoint : line) {
      int glyph_index = raylib::GetGlyphIndex(theme.font, codepoint);
      const auto &glyph_rectangle = theme.font.recs[glyph_index];
      width += glyph_rectangle.width + theme.glyph_spacing;
      height = std::max(height, glyph_rectangle.height);
    }
    width -= theme.glyph_spacing;

    return {.x = width, .y = height};
  }
};  // namespace raylib_helper
