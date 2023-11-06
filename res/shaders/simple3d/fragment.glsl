#version 400

in vec2 frag_texture;

out vec4 fragColor;

uniform sampler2D Texture;

void main(){
  fragColor = texture(Texture, frag_texture);
//  fragColor = vec4(frag_texture.x, frag_texture.y, 0, 1);
}

