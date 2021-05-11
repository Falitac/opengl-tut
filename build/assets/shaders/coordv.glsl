#version 450 compatibility

layout(location = 0) in vec3 position;
out vec4 vertexColor;

uniform mat4 view;
uniform mat4 projection;


void main() {
  mat4 MVP = projection * view;
  gl_Position = MVP * vec4(position, 1.0);
  vertexColor = gl_Color;
}