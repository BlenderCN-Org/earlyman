#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 _uv;

void main() {
  _uv = uv;
  gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}
