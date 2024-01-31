#type vertex
#version 330 core
layout(location=0) in vec3 a_Position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

void main()
{
	gl_Position = projection * view * transform * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core
out vec4 FragColor;
uniform vec4 color;

void main()
{
	FragColor = color;
}