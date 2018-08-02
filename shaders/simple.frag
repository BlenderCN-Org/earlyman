#version 330 core

precision highp float;

uniform sampler2D u_sampler;

in vec2 _uv;
in vec3 _pos;

out vec3 color;

void main(void) {
  // opengl loads images with origin bottom left, so image is vertically mirrored, we correct that here
  color = texture(u_sampler, vec2(_uv.x, 1. - _uv.y)).xyz;
  if(_pos.z > 0.) {
    color = vec3(1., 1., 0.);
  } else {
    color = vec3(0., 1., 0.);
  }
}
