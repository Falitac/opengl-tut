#version 450 compatibility

out vec3 color;
in vec3 fragmentColor;

void main() {
  color = fragmentColor;
}