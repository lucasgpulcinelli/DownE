#version 400

in vec2 point;
in vec3 color;
uniform mat2 mat_rotation;
uniform mat2 mat_scale;
uniform mat4 mat_translation;

out vec3 fragment_color;

void main(){
  fragment_color = color;
  gl_Position = mat_translation*vec4(mat_rotation*mat_scale*point, 0.0, 1.0);
}
