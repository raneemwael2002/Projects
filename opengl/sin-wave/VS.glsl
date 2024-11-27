#version 330

layout (location = 0) in vec4 vertex_position;  
layout (location = 1) in vec4 vertex_color;     
uniform float amplitude;  
uniform float frequency;  

out vec4 frag_color;  // Output fragment color

void main()
{
   
    float y = amplitude * sin(frequency * vertex_position.x * 3.14159f); 
   
    gl_Position = vec4(vertex_position.x, y, 0.0f, 1.0f); 

    frag_color = vertex_color;
}
