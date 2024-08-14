//-------------------------
// - Hazel 2D-
// Renderer2D Circle Shader
//-------------------------
#type vertex
#version 450 core
//版本原因 450 支持flat 330 好像不支持
layout(location=0) in vec3 a_WorldPosition;
layout(location=1) in vec3 a_LocalPosition;
layout(location=2) in vec4 a_Color;
layout(location=3) in float a_Thickness;
layout(location=4) in float a_Fade;
layout(location=5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	float Thickness;
	vec4 Color;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
    Output.Color = a_Color;
	Output.LocalPosition = a_LocalPosition;
    Output.Thickness = a_Thickness;
    Output.Fade = a_Fade;
	v_EntityID = a_EntityID;
    
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core
//out vec4 FragColor;
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int entityId;

struct VertexOutput
{
	vec3 LocalPosition;
	float Thickness;
	vec4 Color;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

void main()
{
	//Calculate distance and fill circle with color
	float distance = 1.0 - length(Input.LocalPosition);
	float circleAlpha = smoothstep(0.0, Input.Fade, distance);
	circleAlpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);
	if (circleAlpha == 0)
		discard;

	//Set output color
	fragColor = Input.Color;
	fragColor.a *= circleAlpha;

	entityId = v_EntityID;
}
