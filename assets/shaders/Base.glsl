#version 330 core

#shader vertex
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texCoord;


uniform mat4 u_viewProjection;
uniform mat4 u_model;

out vec2 v_texCoord;

void main()
{
	v_texCoord = a_texCoord;
	gl_Position = u_viewProjection * u_model * a_position;
}

#shader fragment
layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 u_color;
uniform sampler2D u_texture;


void main()
{
	color = texture(u_texture, v_texCoord) * u_color;
}