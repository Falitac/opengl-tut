#version 450 compatibility

out vec4 color;
in vec4 vertexColor;

void main() {
  color = vertexColor;
}