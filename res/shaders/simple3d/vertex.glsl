#version 400

in vec3 point;
in vec2 texture;

uniform float angle;

out vec2 frag_texture;

void main(){
  frag_texture = texture;
  gl_Position = vec4(mat3(cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle))*point, 1.0);
}
