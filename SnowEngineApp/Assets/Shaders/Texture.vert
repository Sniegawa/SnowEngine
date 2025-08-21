#version 330 core
				
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TextureIndex;

uniform mat4 u_ViewProjection;

//TODO output as struct
out vec2 v_TexCoord;
out vec4 v_Color;
flat out int v_TextureIndex;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position,1.0);
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TextureIndex = a_TextureIndex;
}