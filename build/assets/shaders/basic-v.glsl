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

const float pi = 3.14159265;
//const float frequency = 0.5;

void main() {
  float lightMinDarkness = 0.1;
  fragmentColor = vec3(1.0);
  mat4 MVP = projection * view;

  float frequency = 1 / time + 1.0;
  float amplitude = 0.4*sin(-pi*length(position - vec3(3.0, 3.0, 3.0))*frequency + time);
  gl_Position = MVP * vec4(position, 1.0);

  float radius = 80.0;
  vec3 light = vec3(1.0);
  light.x = cos(time);
  light.z = sin(time);
  light *= radius;
  light.y = 0.0;
  vec3 lightNormal = normalize(light - gl_Position.xyz);

  lightValue = clamp(dot(normal, lightNormal), 0.0, 1.0);

  vec3 lightPositionDiff = light - gl_Position.xyz;
  float dist = dot(lightPositionDiff, lightPositionDiff);
  lightValue = lightValue * (1.0 - lightMinDarkness) + lightMinDarkness;
  fragmentUV = uv;
}