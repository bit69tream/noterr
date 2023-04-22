#pragma once

#include "raylib.hxx"

namespace raylib_helper {
using namespace raylib;

Rectangle from_vectors(Vector2 position, Vector2 dimensions);
Rectangle into_proper_rectangle(Rectangle rectangle);
Rectangle map_rectangle_into_world_coordinates(Rectangle rectangle,
                                               Camera2D camera);
}; // namespace raylib_helper
