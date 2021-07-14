#version 450 core

out vec4 color;
in vec3 fragmentColor;
in vec2 fragmentUV;
in float lightValue;

uniform sampler2D texture1;
uniform float time;

float sharpnessKernel[9] = {
  -1, -1, -1,
  -1,  8, -1,
  -1, -1, -1
};

float blurKernel[9] = {
  1, 1, 1,
  1, 1, 1,
  1, 1, 1
};

void main() {
  color = texture(texture1, fragmentUV);
  vec4 c;
  float dx = abs(mod(time, 6.0) - 3.0);
  float delta = (dx + 1) / 50.0;
  int index = 8;
  for(int y = -1; y <= 1; y++) {
    for(int x = -1; x <= 1; x++) {
      c += sharpnessKernel[index--] * texture(texture1, fragmentUV + delta * vec2(x, y));
    }
  }
  c /= 9.0;
  color = texture(texture1, fragmentUV);
  //color *= lightValue;
}