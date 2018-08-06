#version 300 es

precision highp float;

uniform sampler2D u_sampler;

in vec2 _uv;

out vec3 color;

void main(void) {
  // fix for opengl loading image with origin bottom left, so image is vertically mirrored
  color = texture(u_sampler, vec2(_uv.x, _uv.y)).xyz;
}
