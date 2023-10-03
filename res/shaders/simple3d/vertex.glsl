#version 400

in vec3 point;
in vec2 color;

uniform float angle;

out vec2 frag_color;

void main(){
  frag_color = color;
  gl_Position = vec4(mat3(1, 0, 0, 0, cos(angle), sin(angle), 0, -sin(angle), cos(angle))*point, 1.0);
}
