#version 410
#extension GL_NV_shadow_samplers_cube : enable

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 color;
uniform samplerCube envMap;

out vec4 FragmentColor;

void main()
{
	//SKYBOX
	vec4 envTex = textureCube(envMap, -Normal);
	vec3 final = color.xyz * envTex.xyz;
	
	//FOGGY FUGUE
	vec3 fogColor = vec3(0.7,0.9,1);	
	float u = 0.8;
	final = final * (1-u) + fogColor * u;
	
	FragmentColor = vec4(final, 1.0);
	
	//TODO fog n stuff
	
	// FragmentColor = vec4(Color.xyz, 1);
	// FragmentColor = vec4(Normal, 1);	
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
	
	// FragmentColor = vec4(viewDir, 1.0);
	// FragmentColor = vec4(red, 1.0);
	
	// FragmentColor = color;	
	// FragmentColor = texture(envMap, red);
}