#version 400

in vec3 fragment_color;
out vec4 fragColor;

void main(){
  fragColor = vec4(fragment_color, 1);
}

