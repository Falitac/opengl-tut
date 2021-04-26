#version 450 compatibility

out vec4 color;
in vec3 fragmentColor;

void main() {
  float r, g, b;
  r = (gl_PrimitiveID % 256) / 255.0f;
  g = ((gl_PrimitiveID / 256) % 256) / 255.0f;
  b = ((gl_PrimitiveID / (256 * 256)) % 256) / 255.0f;
  color = vec4(r, g, b, 1.0);
  color = vec4(fragmentColor, 1.0);
}