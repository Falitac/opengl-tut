#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragmentColor;

uniform vec3 cameraDirection;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main() {
  mat4 MVP = projection * view;
  gl_Position = MVP * vec4(position, 1.0);

  vec3 light = vec3(0.0, -1.0, 0.0);
  float factor = max(dot(normal, light), 0) * 0.8 + 0.2;
  fragmentColor = vec3(
    clamp(uv.x,0.0, 1.0),
    clamp(uv.y,0.0, 1.0),
    0.0
  );
}