#type vertex
#version 330 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoords;
layout(location=3) in float a_TexIndex;
layout(location=4) in float a_TilFactor;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilFactor;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	v_TexCoord = a_TexCoords;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilFactor = a_TilFactor;
	gl_Position = projection * view  * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core

out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilFactor;

uniform sampler2D v_Textures[32];
void main()
{
	int index = int(v_TexIndex);
	FragColor = texture(v_Textures[int(v_TexIndex)], v_TexCoord * v_TilFactor) * v_Color;
	//FragColor = v_Color;
}