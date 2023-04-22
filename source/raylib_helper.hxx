#pragma once

#include <string_view>

#include "raylib.hxx"
#include "theme.hxx"

namespace raylib_helper {
  using namespace raylib;

  Rectangle from_vectors(Vector2 position, Vector2 dimensions);
  Rectangle into_proper_rectangle(Rectangle rectangle);
  Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle,
                                                 Camera2D camera);
  void render_wrapping_text_in_bounds(std::string_view text, Rectangle bounding_box, ui::theme theme);
};  // namespace raylib_helper
