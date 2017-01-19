#version 410

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragmentColor;

void main()
{
    FragmentColor = vec4(Color+vec3(0.2), 1);
	
    //FragmentColor = vec4(0, TexCoord.y, 0, 1);
}
