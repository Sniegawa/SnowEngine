#version 330 core
				
//uniform sampler2D u_Texture;
in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureIndex;

out vec4 color;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TextureIndex)],v_TexCoord) * v_Color;
}