#include "raylib_helper.hxx"

namespace raylib_helper {
Rectangle from_vectors(Vector2 position, Vector2 dimensions) {
  return Rectangle{
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
}; // namespace raylib_helper
