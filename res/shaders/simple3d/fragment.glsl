#version 400

in vec3 frag_position;
in vec2 frag_texture;
in vec3 frag_normal;

uniform sampler2D Texture;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 kd;
uniform vec3 ka;

void main(){
  vec3 fnormal = normalize(frag_normal);
  vec3 light_direction = normalize(light_position - frag_position);
  float illumination = max(dot(fnormal, light_direction), 0);

  vec3 diffuse_light = kd * illumination * light_color;
  vec3 ambient_light = ka * vec3(1, 1, 1);

  gl_FragColor = texture(Texture, frag_texture) * vec4(ambient_light + diffuse_light, 1);
}

