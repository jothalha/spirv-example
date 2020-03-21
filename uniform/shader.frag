#version 450

layout(location = 1) uniform vec3 triangleColor;

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(triangleColor, 1.0);
}
