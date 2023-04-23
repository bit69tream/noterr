#version 330
out vec4 finalColor;

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 screenResolution;
uniform float gridTilePercent;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main()
{
  float gridWidth = screenResolution.x * gridTilePercent;
  float gridHeight = screenResolution.y * gridTilePercent;

  float gridSize = gridWidth;
  if (gridWidth > gridHeight) {
    gridSize = gridHeight;
  }

  float mx = floor(mod(fragTexCoord.x * screenResolution.x, gridSize));
  float my = floor(mod(fragTexCoord.y * screenResolution.y, gridSize));

  if (mx == 0.0 || my == 0.0) {
    finalColor = vec4(0.0);
  } else {
    finalColor = fragColor * texture(texture0, fragTexCoord) * colDiffuse;
  }
}
