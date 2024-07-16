#type vertex
#version 450 core
//版本原因 450 支持flat 330 好像不支持
layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;
layout(location=4) in float a_TillingFactor;
layout(location=5) in int a_EntityID;

uniform mat4 projectionView;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TillingFactor;
out flat int v_EntityID;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TillingFactor = a_TillingFactor;
	v_EntityID = a_EntityID;
    gl_Position = projectionView * vec4(a_Position, 1.0f);
}

#type fragment
#version 450 core
//out vec4 FragColor;
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int FragColor2;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TillingFactor;
in flat int v_EntityID;

uniform sampler2D tex[32]; 

void main()
{
    FragColor = texture(tex[int(v_TexIndex)], v_TexCoord * v_TillingFactor) * v_Color;
	FragColor2 = v_EntityID;
    //FragColor = v_Color;
}
