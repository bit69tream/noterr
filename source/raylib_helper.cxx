#include "raylib_helper.hxx"

namespace raylib_helper {
  Rectangle from_vectors(Vector2 position, Vector2 dimensions) {
    return Rectangle {
      .x = position.x,
      .y = position.y,
      .width = dimensions.x,
      .height = dimensions.y,
    };
  }

  Rectangle into_normalized_rectangle(Rectangle rectangle) {
    if (rectangle.width < 0) {
      rectangle.x -= rectangle.width;
      rectangle.width = -rectangle.width;
    }

    if (rectangle.height < 0) {
      rectangle.y -= rectangle.height;
      rectangle.height = -rectangle.height;
    }

    return rectangle;
  }
}
