#version 450
#extension GL_NV_shadow_samplers_cube : enable

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 color;
uniform samplerCube envMap;
uniform vec3 cameraForward;

//Post Process
layout(location = 0) out vec4 OutputColor;
layout(location = 1) out vec4 OutputPosition;

void main()
{
	//ENV COLOR
	vec3 dayPos = vec3(1,0,1);
	float t = 1.0;
	t = (dot(normalize(dayPos), cameraForward)+1)*0.5;	
	// vec3 envColor = vec3(0.7, 0.9, 1.0)*(1-t) + vec3(0.7, 0.9, 1.0)*t;
	vec3 envColor = vec3(0.4, 0.4, 1.0)*(1-t) + vec3(1.0,0.5,0.2)*t;

	//SKYBOX
	vec4 envTex = textureCube(envMap, -Normal);
	vec3 final = color.xyz * envTex.xyz * envColor;
	
	//FOGGY FUGUE
	float u = 0.0;
	if(Position.y < -50)
		u = 1.0;
	else
	{
		u = clamp(1.0 - (Position.y+50)*0.002, 0, 1);
	}
	
	final = final*(1-u) + envColor*u;
	
	//OUTPUT	
	OutputColor = vec4(final, 1.0);
	OutputPosition = vec4(Normal, 1.0);
	
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