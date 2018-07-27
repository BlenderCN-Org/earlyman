#version 330 core

precision highp float;

uniform sampler2D u_sampler;

in vec2 _uv;

out vec3 color;

void main(void) {
  color = texture(u_sampler, _uv).xyz;
}
