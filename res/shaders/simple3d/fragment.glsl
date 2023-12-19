#version 400

in vec3 frag_position;
in vec2 frag_texture;
in vec3 frag_normal;

uniform sampler2D Texture;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 ambient_light_color;
uniform vec3 kd;
uniform vec3 ka;
uniform vec3 ks;
uniform float ns;
uniform vec3 view_position;

void main(){
  vec3 fnormal = normalize(frag_normal);
  vec3 light_direction = normalize(light_position - frag_position);
  float decay = max(length(light_position - frag_position)/2, 0.5);

  float illumination = max(dot(fnormal, light_direction), 0) / decay;
  vec3 diffuse_light = illumination * kd * light_color;

  vec3 ambient_light = ka * ambient_light_color;

  vec3 view_direction = normalize(view_position - frag_position);
  vec3 reflection_direction = normalize(reflect(-light_direction, fnormal));
  float spec = pow(max(dot(view_direction, reflection_direction), 0.0), ns);
  vec3 specular_light = ks * spec * light_color / decay;

  gl_FragColor = texture(Texture, frag_texture)
    * vec4(ambient_light + diffuse_light + specular_light, 1);
}

