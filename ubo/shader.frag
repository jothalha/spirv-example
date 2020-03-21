#version 450

layout(binding = 0) uniform color_state {
  vec3 triangleColor;
};

layout(location = 0) out vec4 outColor;

void main()
{
  outColor = vec4(triangleColor, 1.0);
}
