#Vertex Shader
#version 330 core
layout(location = 0)in vec2 position;
layout(location = 1)in vec2 texCoords;
uniform mat3 MVP = mat3(
	1, 0, 0,
	0, 1, 0,
	0, 0, 1
);
out vec2 v_texCoords;
void main()
{
	v_texCoords = texCoords;
	gl_Position.xy = (vec3(position, 1) * MVP).xy;
	gl_Position.xy = position;
}

#Fragment Shader
#version 330 core
out vec4 color;
in vec2 v_texCoords;
uniform sampler2D samp;
void main()
{
	color = texture(samp, v_texCoords);
}