#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragmentColor;
out vec2 fragmentUV;
out float lightValue;

uniform vec3 cameraDirection;
uniform vec3 cameraPosition;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float time;


void main() {
  float lightMinDarkness = 0.1;
  fragmentColor = vec3(1.0);
  mat4 MVP = projection * view;
  gl_Position = MVP * vec4(position, 1.0);

  float radius = 5.0;
  vec3 light = vec3(1.0);
  light.y = cos(time);
  light.z = sin(time);
  light *= radius;
  light.x = 0.0;
  vec3 lightNormal = normalize(light - position);

  lightValue = clamp(dot(normal, lightNormal), 0.0, 1.0);

  vec3 lightPositionDiff = light - position;
  float dist = dot(lightPositionDiff, lightPositionDiff);
  //lightValue /= dist;
  lightValue = lightValue * (1.0 - lightMinDarkness) + lightMinDarkness;
  fragmentUV = uv;
}