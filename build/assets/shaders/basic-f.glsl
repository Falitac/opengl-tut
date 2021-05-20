#version 450 core

out vec4 color;
in vec3 fragmentColor;
in vec2 fragmentUV;
in float lightValue;

uniform sampler2D texture1;

void main() {
  color = texture(texture1, fragmentUV) * lightValue; 
  color.r = fragmentUV.x;
  color.g = fragmentUV.y;
  color.b = fragmentUV.y * fragmentUV.x;
  color *= lightValue;
}