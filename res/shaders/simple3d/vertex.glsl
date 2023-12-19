#version 400

in vec3 point;
in vec2 texture;
in vec3 normal;

uniform vec3 angle;
uniform vec3 scale;
uniform vec3 position;
uniform vec2 camera_angle;
uniform vec3 view_position;
uniform float ar;

out vec3 frag_position;
out vec2 frag_texture;
out vec3 frag_normal;

void main(){
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

  mat4 model_matrix = rx*ry*s_tr;

  vec4 world_coordinates = vec4(point, 1)*model_matrix;


  vec3 n = vec3(
    cos(camera_angle.y)*sin(camera_angle.x),
    sin(camera_angle.y),
    cos(camera_angle.y)*cos(camera_angle.x)
  );

  vec3 u = normalize(cross(vec3(0, 1, 0), n));
  vec3 v = normalize(cross(n, u));

  mat4 view_matrix = mat4(
    u.x, u.y, u.z, -dot(u, view_position),
    v.x, v.y, v.z, -dot(v, view_position),
    n.x, n.y, n.z, -dot(n, view_position),
    0, 0, 0, 1
  );

  float znear = 1;
  float zfar = 40;
  float fov = 3.14/4;

  mat4 project_matrix = mat4(
    1/(tan(fov/2)*ar), 0, 0, 0,
    0, 1/tan(fov/2), 0, 0,
    0, 0, (zfar+znear) / (znear - zfar), -1,
    0, 0, 2*znear*zfar / (znear - zfar), 0
  );

  gl_Position = world_coordinates*view_matrix*project_matrix;

  frag_position = world_coordinates.xyz;
  frag_texture = texture;
  frag_normal = (vec4(normal, 1)*rx*ry).xyz;
}
