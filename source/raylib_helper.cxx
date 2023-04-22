#include <algorithm>

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
  void render_wrapping_text_in_bounds(std::string_view text, Rectangle bounding_box, ui::theme theme) {
    const float glyph_scale_factor = theme.font_size / theme.font.baseSize;
    const float initial_x_position = bounding_box.x + theme.font_spacing;

    float current_line_max_glyph_height = 0;
    Vector2 next_position = {.x = initial_x_position, .y = bounding_box.y};
    for (const char codepoint : text) {
      const int glyph_index = GetGlyphIndex(theme.font, codepoint);
      const Rectangle &glyph_box = theme.font.recs[glyph_index];
      const float glyph_width = (glyph_box.width * glyph_scale_factor);
      const float glyph_height = (glyph_box.height * glyph_scale_factor);

      current_line_max_glyph_height = std::max(glyph_height, current_line_max_glyph_height);

      if (codepoint == '\n') {
        next_position.x = initial_x_position;
        next_position.y += current_line_max_glyph_height;
        current_line_max_glyph_height = 0;
        continue;
      }

      Vector2 current_position = next_position;
      next_position.x += glyph_width + theme.font_spacing;

      bool is_inside_of_bounding_box = CheckCollisionPointRec(next_position, bounding_box);
      if (!is_inside_of_bounding_box) {
        current_position.x = initial_x_position;
        current_position.y += current_line_max_glyph_height;
        current_line_max_glyph_height = glyph_height;

        next_position = current_position;
        next_position.x += glyph_width + theme.font_spacing;
      }

      bool cannot_be_put_inside_bounding_box = (current_position.y + glyph_height) >= (bounding_box.y + bounding_box.height);
      if (cannot_be_put_inside_bounding_box) {
        break;
      }

      DrawTextCodepoint(theme.font, codepoint, current_position, theme.font_size, theme.note_foreground);
    }
  }
};  // namespace raylib_helper
