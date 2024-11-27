#version 330

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_color;

uniform float theta;

out vec4 frag_color;
void main()
{
	gl_Position = vertex_position;

//	float x = vertex_position.x * cos(theta) + vertex_position.y * sin(theta);
//	float y = -vertex_position.x * sin(theta) + vertex_position.y * cos(theta);
	
//	gl_Position = vec4(x/2,y/2,vertex_position.z,1);

	//frag_color = vertex_position;
	frag_color = vertex_color;
}
