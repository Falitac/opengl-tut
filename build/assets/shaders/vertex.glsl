#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 fragmentColor;

uniform vec3 cameraDirection;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;



float mandelbrot(vec2 point, int iteration) {
  int i = 0;
  vec2 pos = vec2(0.0, 0.0);
  while(dot(pos, pos) <= 4.0 && i < iteration) {
    pos =  vec2(
      pos.x*pos.x -pos.y*pos.y + point.x,
      2.0*pos.x*pos.y + point.y
    );
    i += 1;
  }
  return float(i) / float(iteration);
}


void main() {
  mat4 MVP = projection * view * model;
  gl_Position = MVP * vec4(position, 1.0);
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

  /* for mandelbrot
  mat2 scale = mat2(1.0);
  scale *= 1/3.0;
  float it1 = mandelbrot(scale*position.xy, 100);
  float it2 = mandelbrot(scale*position.xz, 100);
  float it3 = mandelbrot(scale*position.yz, 100);
  */
  fragmentColor.x = abs(sin(1000.0*position.x));
  fragmentColor.y = abs(sin(500.0*position.y));
  fragmentColor.z = abs(cos(700.0*position.z));
  fragmentColor = vec3(1.0, 1.0, 1.0);
  vec3 light = vec3(0.0, -1.0, 0.0);
  float factor = max(dot(normal, light), 0) * 0.8 + 0.2;
  fragmentColor = factor * fragmentColor;
}