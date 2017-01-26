#version 450

in vec3 Color;
in vec2 TexCoord;

uniform vec4 color;
uniform sampler2D mainTexture;

out vec4 FragmentColor;

void main()
{
	FragmentColor = vec4(color.xyz * texture2D(mainTexture, TexCoord).xyz, color.w);
	
	//FragmentColor = vec4(1,1,1,1);
	//FragmentColor = vec4(Color.xyz, 1);
	//FragmentColor = vec4(Normal, 1);	
    //FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	//FragmentColor = vec4(0, TexCoord.y, 0, 1);
	//FragmentColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
}