﻿#type vertex
#version 450 core
//版本原因 450 支持flat 330 好像不支持
layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;
layout(location=4) in float a_TillingFactor;
layout(location=5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
    Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
    Output.TexIndex = a_TexIndex;
    Output.TilingFactor = a_TillingFactor;
	v_EntityID = a_EntityID;
    
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
//out vec4 FragColor;
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int FragColor2;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
    float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32]; 

void main()
{
    FragColor = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color;
	FragColor2 = v_EntityID;
    //FragColor = v_Color;
}
