#version 450

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec3 TexCoord;

uniform vec4 color;
uniform sampler2D mainTexture;

out vec4 FragmentColor;

void main()
{
	vec3 final = color.xyz;

	FragmentColor = vec4(final, 1.0);
	
	//FragmentColor = vec4(Color.xyz, 1);
	//FragmentColor = vec4(Normal, 1);	
    //FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	//FragmentColor = vec4(0, TexCoord.y, 0, 1);
	//FragmentColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
}