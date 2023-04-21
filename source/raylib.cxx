#include "raylib.hxx"

namespace raylib {
  Rectangle RectangleFromVectors(Vector2 position, Vector2 dimensions) {
    return Rectangle {
      .x = position.x,
      .y = position.y,
      .width = dimensions.x,
      .height = dimensions.y,
    };
  }
}
