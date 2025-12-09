#version 330 core

#shader vertex
layout(location = 0) in vec4 a_position;

uniform mat4 u_viewProjection;
uniform mat4 u_model;

void main()
{
	gl_Position = u_viewProjection * u_model * a_position;
}
#shader fragment
layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main()
{
	color = u_color;
}