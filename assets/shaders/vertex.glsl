#version 450 compatibility

layout(location = 0) in vec3 position;

out vec3 fragmentColor;

uniform mat4 view;
uniform mat4 projection;


void main() {
  mat4 MVP = projection * view;
  gl_Position = MVP * vec4(position, 1.0);
  //fragmentColor.x = abs(sin((position.y)));
  //fragmentColor.y = abs(cos((position.x + 30.f + 0.3*position.z*position.z)));
  //fragmentColor.z = position.z*position.y;
  fragmentColor = vec3(1.0);

  if(position.y > 0.4) {
    fragmentColor = vec3(1.0, 1.0, 0.0);
  }
  if(position.y >= -0.4 && position.y <= 0.4) {
    fragmentColor = vec3(0.0, 0.0, 0.0);
  }

  float radius = 1.2;
  vec3 abc;
  abc.x = position.x - 6.0;
  abc.y = position.y - 0.0;
  abc.z = position.z - 0.0;

  if(dot(abc, abc) <= radius * radius) {
    fragmentColor = vec3(0.0, 0.0, 0.0);
  }

  radius = 0.7;
  if(dot(abc, abc) <= radius * radius) {
    fragmentColor = vec3(1.0, 1.0, 1.0);
  }

  radius = 0.45;
  if(dot(abc, abc) <= radius * radius) {
    fragmentColor = vec3(0.0, 0.0, 0.0);
  }

  radius = 0.41;
  if(dot(abc, abc) <= radius * radius) {
    fragmentColor = vec3(1.0, 1.0, 1.0);
  }
  fragmentColor.x = 0.5f;
}