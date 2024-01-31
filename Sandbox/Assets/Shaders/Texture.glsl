#type vertex
#version 330 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_TexCoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;
out vec2 v_TexCoord;
void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = projection * view * transform * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core
out vec4 FragColor;
in vec2 v_TexCoord;
uniform vec4 color;
uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, v_TexCoord * 10) * color;
}