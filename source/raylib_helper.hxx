#pragma once

#include <string_view>

#include "raylib.hxx"
#include "theme.hxx"

namespace raylib_helper {
  using namespace raylib;

  Rectangle from_vectors(Vector2 position, Vector2 dimensions);
  Rectangle into_proper_rectangle(Rectangle rectangle);
  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle, Camera2D camera);
  // TODO: this feels like too much for the raylib_helper
  void render_wrapping_text_in_bounds(std::wstring_view text, Rectangle bounding_box, const ui::theme &theme);
  Rectangle subtract_border_from_rectangle(Rectangle rectangle, const ui::theme &theme);
  Rectangle add_border_to_rectangle(Rectangle rectangle, const ui::theme &theme);
  Vector2 get_line_dimensions(std::wstring_view line, const ui::theme &theme);
};  // namespace raylib_helper
