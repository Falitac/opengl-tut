#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragmentColor;
out vec2 fragmentUV;

uniform vec3 cameraDirection;
uniform vec3 cameraPosition;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main() {
  mat4 MVP = projection * view;
  gl_Position = MVP * vec4(position, 1.0);

  vec3 light = vec3(sqrt(3) / 3.0, sqrt(3) / 3.0, -sqrt(3) / 3.0);
  light = -normalize(cameraDirection);
  float factor = clamp(dot(normal, light), 0.0, 1.0) * 0.8 + 0.2;
  fragmentColor = vec3(
    uv.x,
    uv.y,
    1 - uv.x
  );
  fragmentColor = fragmentColor * factor;
  fragmentUV = uv;
}