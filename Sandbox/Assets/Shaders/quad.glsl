#type vertex
#version 330 core
layout(location=0) in vec3 a_Position;
layout(location = 1) in vec2 aTexCoords;
out vec3 v_Position;
out vec2 v_TexCoord;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	v_Position = a_Position;
	v_TexCoord = aTexCoords;
	gl_Position = projection * view * vec4(a_Position, 1.0f);
}


#type fragment
#version 330 core
out vec4 FragColor;
uniform sampler2D tex;
in vec2 v_TexCoord;
void main()
{
	FragColor = texture(tex, v_TexCoord);
}