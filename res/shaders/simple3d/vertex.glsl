#version 400

in vec3 point;
in vec2 texture;

uniform vec3 angle;
uniform vec3 scale;
uniform vec3 position;

out vec2 frag_texture;

void main(){
  frag_texture = texture;

  mat4 s_tr = mat4(
    scale.x, 0,       0,       position.x,
    0,       scale.y, 0,       position.y,
    0,       0,       scale.z, position.z,
    0,       0,       0,       1
  );

  mat4 rx = mat4(
    1, 0,             0,            0,
    0, cos(angle.x), -sin(angle.x), 0,
    0, sin(angle.x),  cos(angle.x), 0, 
    0, 0,             0,            1
  );

  mat4 ry = mat4(
    cos(angle.y), 0, -sin(angle.y), 0,
    0,            1,  0,            0,
    sin(angle.y), 0,  cos(angle.y), 0,
    0,            0,  0,            1
  );

  gl_Position = vec4(point, 1.0)*rx*ry*s_tr;
}
