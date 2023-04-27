#version 330                    // -*- mode: glsl -*-
out vec4 finalColor;

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec4 screenResolutionAndTopLeftPoint;
uniform float gridTilePercent;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main() {
  float gridWidth = (screenResolutionAndTopLeftPoint.x * gridTilePercent);
  float gridHeight = (screenResolutionAndTopLeftPoint.y * gridTilePercent);

  float gridSize = gridWidth;
  if (gridWidth > gridHeight) {
    gridSize = gridHeight;
  }

  float x = ((fragTexCoord.x * screenResolutionAndTopLeftPoint.x) + screenResolutionAndTopLeftPoint.z);
  float y = ((fragTexCoord.y * screenResolutionAndTopLeftPoint.y) + screenResolutionAndTopLeftPoint.w);

  float mx = floor(mod(x, gridSize));
  float my = floor(mod(y, gridSize));

  if (mx == 0.0 || my == 0.0) {
    finalColor = vec4(0.0);
  } else {
    finalColor = fragColor * texture(texture0, fragTexCoord) * colDiffuse;
  }
}
