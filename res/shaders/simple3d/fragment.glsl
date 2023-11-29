#version 400

in vec3 frag_position;
in vec2 frag_texture;
in vec3 frag_normal;

uniform sampler2D Texture;
vec3 light_position = vec3(0, 1, -0.5);

vec3 light_color = vec3(1, 1, 1);
float kd = 2;
float ka = 0.5;

void main(){
  vec3 fnormal = normalize(frag_normal);
  vec3 light_direction = normalize(light_position - frag_position);
  float illumination = max(dot(fnormal, light_direction), 0);

  vec3 diffuse_light = kd * illumination * light_color;
  vec3 ambient_light = ka * light_color;

  gl_FragColor = texture(Texture, frag_texture) * vec4(ambient_light + diffuse_light, 1);
}

