#type vertex
#version 330 core
layout(location=0) in vec3 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;
layout(location=4) in float a_TillingFactor;

uniform mat4 projectionView;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TillingFactor;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TillingFactor = a_TillingFactor;
    gl_Position = projectionView * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TillingFactor;
uniform sampler2D tex[32]; 

void main()
{
    FragColor = texture(tex[int(v_TexIndex)], v_TexCoord * v_TillingFactor) * v_Color;
    //FragColor = v_Color;
}