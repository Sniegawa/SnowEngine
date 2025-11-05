#version 330 core
				
//uniform sampler2D u_Texture;
in vec2 v_TexCoord;
in vec4 v_Color;
flat in int v_TextureIndex;
flat in int v_EntityID;


layout(location = 0) out vec4 color;
layout(location = 1) out int ID;

uniform sampler2D u_Textures[32];

vec4 SampleTexture(int index, vec2 texCoord) {
    switch (index) {
        case 0:  return texture(u_Textures[0], texCoord);
        case 1:  return texture(u_Textures[1], texCoord);
        case 2:  return texture(u_Textures[2], texCoord);
        case 3:  return texture(u_Textures[3], texCoord);
        case 4:  return texture(u_Textures[4], texCoord);
        case 5:  return texture(u_Textures[5], texCoord);
        case 6:  return texture(u_Textures[6], texCoord);
        case 7:  return texture(u_Textures[7], texCoord);
        case 8:  return texture(u_Textures[8], texCoord);
        case 9:  return texture(u_Textures[9], texCoord);
        case 10: return texture(u_Textures[10], texCoord);
        case 11: return texture(u_Textures[11], texCoord);
        case 12: return texture(u_Textures[12], texCoord);
        case 13: return texture(u_Textures[13], texCoord);
        case 14: return texture(u_Textures[14], texCoord);
        case 15: return texture(u_Textures[15], texCoord);
        case 16: return texture(u_Textures[16], texCoord);
        case 17: return texture(u_Textures[17], texCoord);
        case 18: return texture(u_Textures[18], texCoord);
        case 19: return texture(u_Textures[19], texCoord);
        case 20: return texture(u_Textures[20], texCoord);
        case 21: return texture(u_Textures[21], texCoord);
        case 22: return texture(u_Textures[22], texCoord);
        case 23: return texture(u_Textures[23], texCoord);
        case 24: return texture(u_Textures[24], texCoord);
        case 25: return texture(u_Textures[25], texCoord);
        case 26: return texture(u_Textures[26], texCoord);
        case 27: return texture(u_Textures[27], texCoord);
        case 28: return texture(u_Textures[28], texCoord);
        case 29: return texture(u_Textures[29], texCoord);
        case 30: return texture(u_Textures[30], texCoord);
        case 31: return texture(u_Textures[31], texCoord);
    }
    return vec4(1.0, 0.0, 1.0, 1.0); // fallback magenta if out-of-bounds
}


void main()
{
  	vec4 texColor = SampleTexture(v_TextureIndex, v_TexCoord);

       if(texColor.a == 0.0)
    {
      discard;
    }
    color = texColor * v_Color;

    ID = v_EntityID;
}
