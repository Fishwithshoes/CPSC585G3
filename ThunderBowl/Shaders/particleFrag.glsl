#version 450

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 color;
uniform sampler2D mainTexture;
uniform float fogLevel;

uniform vec3 cameraPos;
uniform vec3 cameraForward;

//Post Process
layout(location = 0) out vec4 OutputColor;
layout(location = 1) out vec4 OutputPosition;
layout(location = 2) out vec4 OutputNormal;

void main()
{
	//ENV COLOR
	vec3 dayPos = vec3(1,0,1);
	float t = 1.0;
	t = (dot(normalize(dayPos), cameraForward)+1)*0.5;	
	// vec3 envColor = vec3(0.7, 0.9, 1.0)*(1-t) + vec3(0.7, 0.9, 1.0)*t;
	vec3 envColor = vec3(0.4, 0.4, 1.0)*(1-t) + vec3(1.0,0.5,0.2)*t;
	
	if(cameraPos.y < -2.0)
	{
		float v = clamp(0.0 - Position.y*0.0003, 0.0, 1.0);
		vec3 newEnvColor = (vec3(0.0, 1.0, 1.0)*0.4)*0.5 + envColor*0.5;
		envColor = (newEnvColor*(1-v) + vec3(0)*v);
	}
	
	//WORLD CALCULATIONS
	vec3 viewDir = normalize(cameraPos-Position);
	float viewDist = length(cameraPos-Position);
	
	vec4 mainTex = texture2D(mainTexture, TexCoord);
	
	vec4 final = color * mainTex;
	
	//FOGGY FUGUE
	float u = clamp(viewDist*0.003*fogLevel, 0, 1);
	final.xyz = final.xyz * (1-u) + envColor * u;
	
	//OUTPUT
	OutputColor = vec4(final);
	OutputPosition = vec4(Position*0.0005+0.5, 1.0);
	OutputNormal = vec4(Normal*0.5+0.5, 1.0);
	
	// OutputColor = vec4(Color.xyz, 1);
	// OutputColor = vec4(Normal, 1);	
    // OutputColor = vec4(TexCoord.x, 0, 0, 1);
	// OutputColor = vec4(0, TexCoord.y, 0, 1);
	// OutputColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
	// OutputColor = vec4(1,1,1, color.w);
	// OutputColor = vec4(mainTex.w, 0, 0, 1);
	// OutputColor = vec4(cameraPos, 1);
	// OutputColor = vec4(fogLevel, 0, 0, 1);
	// OutputColor = vec4(viewDist);
}