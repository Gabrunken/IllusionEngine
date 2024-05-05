-- - vertex_shader-- -

#version 440 core

in vec4 inVertexPosition;

void main()
{
	gl_Position = inVertexPosition;
}

-- - fragment_shader-- -

#version 440 core

out vec4 outColor;

uniform vec3 inColor;

void main()
{
	outColor = vec4(1.0, 0.0, 1.0, 1.0);
}