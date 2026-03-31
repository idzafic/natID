//Created by IDz on 20240116
#version 410
flat in vec3 color;

out vec4 frag_color;

void main() 
{
  frag_color = vec4(color, 1.0);
}
