#version 450 core

out vec4 color;
in vec3 fragmentColor;
in vec2 fragmentUV;

uniform sampler2D texture1;

void main() {
  color = vec4(fragmentColor, 1.0);
  color = texture(texture1, fragmentUV);
}