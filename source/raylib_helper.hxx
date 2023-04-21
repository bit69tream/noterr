#pragma once

#include "raylib.hxx"

namespace raylib_helper {
  using namespace raylib;

  Rectangle from_vectors(Vector2 position, Vector2 dimensions);
  Rectangle into_proper_rectangle(Rectangle rectangle);
}
