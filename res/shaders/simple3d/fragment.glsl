#version 400

in vec2 frag_color;

out vec4 fragColor;

void main(){
  fragColor = vec4(frag_color.x, frag_color.y, 0, 1);
}

