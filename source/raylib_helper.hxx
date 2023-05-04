#pragma once

#include <string_view>

#include "raylib.hxx"
#include "theme.hxx"

namespace raylib_helper {
  using namespace raylib;

  Rectangle from_vectors(Vector2 position, Vector2 dimensions);

  Rectangle into_a_rectangle_where_the_top_left_corder_is_actually_a_top_left_corner(Rectangle rectangle);

  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle, Camera2D camera);

  Rectangle subtract_border_from_rectangle(Rectangle rectangle, const ui::theme &theme);
  Rectangle add_border_to_rectangle(Rectangle rectangle, const ui::theme &theme);

  Vector2 get_line_dimensions(std::wstring_view line, const ui::theme &theme);
};  // namespace raylib_helper
