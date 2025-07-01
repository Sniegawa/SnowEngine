#version 330 core
				
uniform sampler2D u_Texture;
uniform vec3 u_Tint;
in vec2 v_TexCoord;

out vec4 color;

void main()
{
	color = texture(u_Texture,v_TexCoord) * vec4(u_Tint,1.0);
}